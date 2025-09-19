#include "search.hpp"

#include "eval.hpp"
#include "pick.hpp"
#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"
#include "tt.hpp"

#include <cassert>
#include <chrono>
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


std::optional<Move> Search::FindBestMove(PositionFixedMemory& pos) 
{
    auto start = std::chrono::high_resolution_clock::now();

    Move best_move;
    int best_score = -INF;

    nodes = 0;
    tt_cuts = 0;

    eval.Init(pos);

    MoveGenerator<MoveGenType::NotForced> gen(pos);
    if(gen.moves.empty())
        return std::nullopt;

    MovePicker picker(gen.moves, pos);

    while(std::optional __move = picker.next()) 
    {
        const Move& move = __move.value();

        pos.do_move(move);
        eval.Update(move);

        int score = -negamax(pos, maxDepth - 1, -INF, -best_score);

        pos.undo_move();
        eval.Rollback();

        if(score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::format(
        "---------------\n"
        "Time: {}\nNodes: {}\nScore: {}\nTT Cuts: {}\n",
        std::chrono::duration<double>(end - start).count(), 
        nodes, 
        best_score,
        tt_cuts
    );

    return best_move;
}

int Search::negamax(PositionFixedMemory& pos, int depth, int alpha, int beta) 
{
    ProbeResult probe = tt.probe(pos.get_hash(), depth, alpha, beta);

    if(probe.score) {
        tt_cuts++;
        return *probe.score;
    }


    if(depth <= 0)
        return qsearch(pos, alpha, beta);

    if(pos.is_draw(*globalHistory)) 
        return DRAW_SCORE;

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