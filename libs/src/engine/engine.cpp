#include "engine.hpp"

#include "logic/position.hpp"
#include <optional>
#include <thread>

namespace game::engine
{

using namespace logic;

Engine& Engine::SetPosition(const PositionDynamicMemory& pos) {
    this->pos = &pos;
    search.SetGlobalHistory(pos.get_history());
    return *this;
}

Engine& Engine::SetMaxDepth(int depth) {
    if(depth >= MAX_HISTORY_SIZE) {
        throw std::runtime_error(std::format(
            "depth cant be more than {}\n", 
            MAX_HISTORY_SIZE
        ));
    }
    
    search.SetMaxDepth(depth);

    return *this;
}

Engine& Engine::SetTTSizeMB(size_t mb) {
    search.SetTTSizeMB(mb);
    return *this;
}

void Engine::StartWorker(const std::function<void(logic::Move)>& onMoveFound) 
{
    mainWorker = std::thread(
        [this, onMoveFound]() 
        {
            while(!stop)
            {
                std::unique_lock<std::mutex> ul(mtx);
                cv.wait(ul, [this](){return can_search || stop;});

                if(stop) 
                    break;

                PositionFixedMemory pos(*this->pos);
                if(std::optional move = search.FindBestMove(pos)) {
                    can_search = false;
                    onMoveFound(*move);
                }
            }
        }
    );
}

void Engine::StopWorker() {
    stop = true;
    cv.notify_one();

    if(mainWorker.joinable()) 
        mainWorker.join();
}

void Engine::FindBestMove() {
    can_search = true;
    cv.notify_one();
}

}