#pragma once 

#include "eval.hpp"
#include "tt.hpp"
#include "timer.hpp"
#include "logic/move.hpp"
#include "logic/position.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Core::Engine
{

/* 
В Launch запускается поток, который ждет сигнала на старт поиска.
В Think посылается сигнал на старт поиска.
Когда поиск закончен, вызывается callback onBestMove.
*/
class Search {
public:

    struct Info {
        long long nodes;
        long long tt_cuts;
        std::chrono::seconds time;
        int depth;
        int eval;
        Logic::Move bestMove;
    };
    struct Options {
        uint64_t timeSec;
        uint64_t ttSizeMB;
        int maxDepth;
        std::function<void(Info)> onBestMove;
        const Logic::PositionDM* pos;
    };

public:

    Search();
    ~Search() {Stop();}

    void Launch(const Options&);
    void Think();
    void Stop();

private:

    bool iterativeDeepening();
    int negamax(Logic::PositionFM&, int depth, int alpha, int beta);
    int qsearch(Logic::PositionFM&, int alpha, int beta);

private:
    
    Info info;
    Timer timer;
    Transposition tt;
    Evaluation eval;
    const Logic::PositionDM* rootPos;
    int maxDepth;
    std::thread searchThread;
    bool stopSearch;
    bool allowedToSearch;
    std::mutex mtx;
    std::condition_variable cv;

};

}