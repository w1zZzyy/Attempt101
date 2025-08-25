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
    RootMove SearchMoves(PositionFixedMemory& pos, int alpha, int beta, Func&& SearchFunc);

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
Search::RootMove Search::SearchMoves(PositionFixedMemory& pos, int alpha, int beta, Func&& SearchFunc) 
{
    using namespace logic;

    nodes++;
    pos.update();

    RootMove rm;

    if(pos.is_draw()) {
        rm.score = 0;
        return rm;
    }

    MoveList moves;
    moves.generate<MGT>(pos);

    if(moves.empty()) {
        if(pos.is_check()) 
            rm.score = -INF + pos.get_ply();
        else if constexpr (MGT == MoveGenType::Forced) 
            rm.score = eval.score();
        else 
            rm.score = 0;
        return rm;
    } 

    rm.score = alpha;

    eval.push();
    orderer.OrderCaptures(moves);

    const int score_curr = eval.score();

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(move);


        int score = std::forward<Func>(SearchFunc)(pos, rm.score, beta);
        if(score > rm.score) {
            rm.score = score;
            rm.move = move;
        }

        pos.undo_move();
        eval.rollback();

        if(rm.score >= beta) 
            break;
    }

    eval.pop();

    return rm;
}

}