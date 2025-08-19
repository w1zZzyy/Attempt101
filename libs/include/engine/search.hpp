#pragma once

#include "eval.hpp"
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>

namespace game::engine
{

/* 
- setup search params 
- start serach worker
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
    Search& StartSearchWorker(const std::function<void(RootMove)>& callback);
    void FindBestMove(const std::string& fen);
    void Stop();

private:

    std::optional<RootMove> BestMove();
    int Negamax(PositionFixedMemory& pos, int depth, int alpha, int beta);

private:

    int maxDepth;
    Eval eval;

    std::string fen;
    std::vector<RootMove> variations;

    std::thread search_thread;
    std::condition_variable cv;
    std::mutex mtx;
    std::atomic<bool> can_search{false};
    std::atomic<bool> stop{false};

};


}