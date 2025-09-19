#include "search.hpp"

#include "eval.hpp"
#include "pick.hpp"
#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"
#include "tt.hpp"

#include <cassert>
#include <cstdint>
#include <optional>

namespace game::engine
{

using namespace logic;

Search& Search::SetGlobalHistory(const StateStorage<DynamicStorage>& gh) noexcept {
    globalHistory = &gh;
    return *this;
}

Search& Search::SetMaxDepth(int depth) noexcept {
    maxDepth = depth;
    return *this;
}

Search& Search::SetTTSizeMB(size_t mb) {
    tt.resize(mb);
    return *this;
}

Search& Search::SetTimeLimit(uint64_t sec) noexcept {
    timer.setLimit(sec);
    return*this;
}


std::optional<Move> Search::FindBestMove(PositionFixedMemory& pos) 
{
    timer.Start();

    Move best_move;
    int best_score = -INF;

    int depth;
    nodes = 0;
    tt_cuts = 0;

    eval.Init(pos);

    MoveGenerator<MoveGenType::NotForced> gen(pos);
    if(gen.moves.empty())
        return std::nullopt;

    MovePicker picker(gen.moves, pos);

    for(depth = 1; depth <= maxDepth; ++depth)
    {
        int iter_best_score = -INF;
        Move iter_best_move;

        while(std::optional __move = picker.next()) 
        {
            const Move& move = __move.value();

            pos.do_move(move);
            eval.Update(move);

            int score = -negamax(pos, depth - 1, -INF, -iter_best_score);

            pos.undo_move();
            eval.Rollback();

            if(timer.TimeUp()) {
                goto search_end;
            }

            if(score > iter_best_score) {
                iter_best_score = score;
                iter_best_move = move;
            }
        }

        picker.update(iter_best_move);
        best_score = iter_best_score;
        best_move = iter_best_move;

    }

    search_end:

    std::cout << std::format(
        "---------------\n"
        "Time: {}\nDepth: {}\nNodes: {}\nScore: {}\nTT Cuts: {}\n",
        timer.TimePassed(),
        depth,
        nodes, 
        best_score,
        tt_cuts
    );

    return best_move;
}

int Search::negamax(PositionFixedMemory& pos, int depth, int alpha, int beta) 
{
    if(timer.TimeUp()) 
        return 0;

    if(pos.is_draw(*globalHistory)) 
        return DRAW_SCORE;

    ProbeResult probe = tt.probe(pos.get_hash(), depth, alpha, beta);

    if(probe.score) {
        tt_cuts++;
        return *probe.score;
    }


    if(depth <= 0)
        return qsearch(pos, alpha, beta);

    nodes++;

    MoveGenerator<MoveGenType::NotForced> gen(pos);

    if(gen.moves.empty()) {
        if(pos.is_check()) return -INF + pos.get_ply();
        return DRAW_SCORE;
    }

    MovePicker moveloads(gen.moves, pos, probe.move);


    const int oldAlpha = alpha;
    int bestScore = -INF;
    logic::Move bestMove;


    while(std::optional __move = moveloads.next())
    {
        const Move& move = __move.value();

        pos.do_move(move);
        eval.Update(move);

        int score = -negamax(pos, depth - 1, -beta, -alpha);

        pos.undo_move();
        eval.Rollback();

        if(timer.TimeUp()) 
            return 0;

        if(score > bestScore) {
            bestScore = score;
            bestMove = move;
            if(bestScore > alpha) {
                alpha = bestScore;
                if(alpha >= beta) {
                    tt.store(pos.get_hash(), bestScore, move, depth, EntryType::LowerBound);
                    return bestScore;
                }
            }
        }
    }

    if(bestScore <= oldAlpha)
        tt.store(pos.get_hash(), bestScore, bestMove, depth, EntryType::UpperBound);
    else
        tt.store(pos.get_hash(), bestScore, bestMove, depth, EntryType::Exact);

    return bestScore;
}

int Search::qsearch(PositionFixedMemory& pos, int alpha, int beta)
{
    if(timer.TimeUp()) 
        return 0;

    if(pos.is_draw(*globalHistory)) 
        return DRAW_SCORE;

    nodes++;

    int score = eval.Score();

    if(pos.get_ply() == MAX_HISTORY_SIZE - 1)
        return score;

    if(score >= beta) 
        return beta;

    if(score > alpha)
        alpha = score;

    
    MoveGenerator<MoveGenType::Forced> gen(pos);

    if(gen.moves.empty()) {
        if(pos.is_check()) return -INF + pos.get_ply();
        return score;
    }

    MovePicker moveloads(gen.moves, pos);


    while(std::optional __move = moveloads.next())
    {
        const Move& move = __move.value();

        pos.do_move(move);
        eval.Update(move);

        score = -qsearch(pos, -beta, -alpha);

        pos.undo_move();
        eval.Rollback();

        if(score > alpha) {
            alpha = score;
            if(alpha >= beta)
                return beta;
        }
    }

    return alpha;
}

}