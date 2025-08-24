#pragma once

#include "eval.hpp"
#include "moveordering.hpp"
#include "tt.hpp"

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

namespace game::engine
{

/* 
- setup search params (depth, tt size)
- start search worker
- call find best move 
P.S
when search is finished, callback which u passed to 
'StartSearchWWorker' will be called
*/
class Search {
public:

    struct RootMove {
        logic::Move move;
        int score;
    };

public:

    Search() {Eval::Setup();}
    ~Search() {Stop();}
    Search& SetMaxDepth(int d);
    Search& SetTableSize(size_t mb);
    Search& StartSearchWorker(const std::function<void(RootMove)>& callback);
    void FindBestMove(const std::string& fen);
    void Stop();

private:

    std::optional<RootMove> BestMove();
    int Negamax(PositionFixedMemory& pos, int depth, int alpha, int beta);
    int QSearch(PositionFixedMemory& pos, int alpha, int beta);

    template<logic::MoveGenType MGT, typename Func>
    int SearchMoves(PositionFixedMemory& pos, int alpha, int beta, Func&& SearchFunc);

private:

    long long nodes = 0;
    int maxDepth;

    Eval eval;
    MoveOrderer orderer;
    Transpositions tt;

    std::string fen;

    std::thread search_thread;
    std::condition_variable cv;
    std::mutex mtx;
    std::atomic<bool> can_search{false};
    std::atomic<bool> stop{false};

};

template<logic::MoveGenType MGT, typename Func>
int Search::SearchMoves(PositionFixedMemory& pos, int alpha, int beta, Func&& SearchFunc) 
{
    using namespace logic;

    nodes++;
    pos.update();

    MoveList moves;
    moves.generate<MGT>(pos);

    if(moves.empty()) {
        if(pos.is_check())
            return -INF + MAX_HISTORY_SIZE;
        return 0;
    } 
    else if(pos.is_draw()) {
        return 0;
    }

    Eval curr(eval);
    orderer.OrderCaptures(moves);

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        int score = SearchFunc(pos, alpha, beta);
        alpha = std::max(alpha, score);

        pos.undo_move();
        eval = curr;

        if(alpha >= beta) {
            return beta;
        }
    }

    return alpha;
}

}