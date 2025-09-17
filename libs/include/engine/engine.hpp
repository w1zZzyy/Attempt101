#pragma once 

#include "logic/position.hpp"
#include "search.hpp"

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace game::engine 
{

class Engine {
public:

    Engine& SetPosition(const PositionDynamicMemory&);
    Engine& SetMaxDepth(int);

    void StartWorker(const std::function<void(logic::Move)>& onMoveFound);
    void StopWorker();
    void FindBestMove();

private:

    const PositionDynamicMemory* pos;
    Search search;

    std::thread mainWorker;
    std::mutex mtx;
    std::condition_variable cv;
    bool can_search{false};
    bool stop{false};

};

}