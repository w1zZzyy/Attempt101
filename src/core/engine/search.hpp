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
        std::function<void(Info)> onMove;
    };

public:

    Search();
    ~Search() {Stop();}

    void Init(const Options&);
    void Launch();
    void Think();
    void Stop();

    void SetPosition(const Logic::PositionDM& pos) noexcept {
        this->rootPos = &pos;
    }

private:

    bool iterativeDeepening();
    int negamax(Logic::PositionFM&, int depth, int alpha, int beta);
    int qsearch(Logic::PositionFM&, int alpha, int beta);

private:
    
    Info info;
    Timer timer;
    Transposition tt;
    Evaluation eval;
    Logic::Move killers[Logic::MAX_HISTORY_SIZE][2];

    const Logic::PositionDM* rootPos;
    int maxDepth;

    std::thread searchThread;
    std::mutex mtx;
    std::condition_variable cv;

    bool stopSearch;
    bool allowedToSearch;

    std::function<void(Info)> onBestMove;
};

}