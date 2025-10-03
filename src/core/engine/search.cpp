#include "search.hpp"
#include "engine/pick.hpp"
#include "logic/movelist.hpp"
#include "logic/position.hpp"
#include <cassert>

namespace Core::Engine
{

Search::Search()
{
    static bool init = false;
    if(!init) {
        Evaluation::Setup();
        init = true;
    }
}

void Search::Launch()
{
    searchThread = std::thread([this]() 
    {
        while(!stopSearch)
        {
            {
                std::unique_lock lock(mtx);
                cv.wait(lock, [this]() {return stopSearch || allowedToSearch;});
            }

            if(allowedToSearch && iterativeDeepening()) {
                onBestMove(info);
                allowedToSearch = false;
            }
        }
    });
}

void Search::Init(const Options& options) 
{
    stopSearch = false;
    allowedToSearch = false;
    maxDepth = options.maxDepth;
    tt.resize(options.ttSizeMB);
    timer.setLimit(options.timeSec);
    onBestMove = std::move(options.onMove);
}

void Search::Think() 
{
    assert(rootPos);

    {
        std::lock_guard lock(mtx);

        if (allowedToSearch) {
        throw std::runtime_error("Search is already in progress");
        }
        if (stopSearch) {
        throw std::runtime_error("Search has been stopped");
        }

        allowedToSearch = true;
    }

    cv.notify_one();
}

void Search::Stop()
{
    {
        std::lock_guard lock(mtx);
        stopSearch = true;
        allowedToSearch = false;
    }
    cv.notify_one();
    if(searchThread.joinable())
        searchThread.join();
}

bool Search::iterativeDeepening()
{
    timer.Start();

    info.eval = -Logic::INF - 1;
    info.time = std::chrono::seconds(0);
    info.depth = 0;
    info.nodes = 0;
    info.tt_cuts = 0;
    info.bestMove = 0;

    Logic::PositionFM pos(*rootPos);
    eval.Init(pos);

    Logic::MoveGenerator<Logic::MoveGenType::All> gen(pos);
    if(gen.moves.empty()) 
        return false;

    MovePicker picker(gen.moves, pos);

    for(int depth = 1; depth <= maxDepth; ++depth) 
    {
        int alpha = -Logic::INF;
        Logic::Move bestMoveThisIter;

        while(std::optional m = picker.next())
        {
            const Logic::Move& move = m.value();

            pos.DoMove(move);
            eval.Update(move);

            int score = -negamax(pos, depth - 1, -Logic::INF, -alpha);

            pos.UndoMove();
            eval.Rollback();

            if(timer.TimeUp()) 
                goto __search_end;

            if(score > alpha) {
                alpha = score;
                bestMoveThisIter = move;
            }
        }

        info.eval = alpha;
        info.depth = depth;
        info.bestMove = bestMoveThisIter;

        picker.update(info.bestMove);
    }

    __search_end:

    info.time = timer.TimePassed();
    return true;
}

int Search::negamax(Logic::PositionFM& pos, int depth, int alpha, int beta)
{
    if(timer.TimeUp()) 
        return 0;

    ProbeResult probe = tt.probe(pos.GetHash(), depth, alpha, beta);

    if(probe.score) {
        info.tt_cuts++;
        return *probe.score;
    }


    if(depth <= 0)
        return qsearch(pos, alpha, beta);

    info.nodes++;

    Logic::MoveGenerator<Logic::MoveGenType::All> gen(pos);
    if(gen.moves.empty()) {
        if(pos.IsCheck()) return -Logic::INF + pos.GetPly();
        return Logic::DRAW_SCORE;
    }

    MovePicker picker(gen.moves, pos, probe.move);


    const int oldAlpha = alpha;
    int bestScore = -Logic::INF;
    Logic::Move bestMove;


    while(std::optional m = picker.next())
    {
        const Logic::Move& move = m.value();

        pos.DoMove(move);
        
        if(pos.IsDraw(rootPos->GetHistory())) {
            pos.UndoMove();
            return Logic::DRAW_SCORE;
        }

        eval.Update(move);

        int score = -negamax(pos, depth - 1, -beta, -alpha);

        pos.UndoMove();
        eval.Rollback();

        if(timer.TimeUp()) 
            return 0;

        if(score > bestScore) {
            bestScore = score;
            bestMove = move;
            if(bestScore > alpha) {
                alpha = bestScore;
                if(alpha >= beta) {
                    tt.store(pos.GetHash(), bestScore, move, depth, EntryType::LowerBound);
                    return bestScore;
                }
            }
        }
    }

    if(bestScore <= oldAlpha)
        tt.store(pos.GetHash(), bestScore, bestMove, depth, EntryType::UpperBound);
    else
        tt.store(pos.GetHash(), bestScore, bestMove, depth, EntryType::Exact);

    return bestScore;
}

int Search::qsearch(Logic::PositionFM& pos, int alpha, int beta)
{
    if(timer.TimeUp()) 
        return 0;

    if(pos.IsDraw(rootPos->GetHistory())) 
        return Logic::DRAW_SCORE;

    info.nodes++;

    int score = eval.Score();

    if(pos.GetPly() == Logic::MAX_HISTORY_SIZE - 1)
        return score;

    if(score >= beta) 
        return beta;

    if(score > alpha)
        alpha = score;

    
    Logic::MoveGenerator<Logic::MoveGenType::Forced> gen(pos);
    if(gen.moves.empty()) {
        if(pos.IsCheck()) return -Logic::INF + pos.GetPly();
        return score;
    }

    MovePicker picker(gen.moves, pos);


    while(std::optional m = picker.next())
    {
        const Logic::Move& move = m.value();

        pos.DoMove(move);
        eval.Update(move);

        score = -qsearch(pos, -beta, -alpha);

        pos.UndoMove();
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