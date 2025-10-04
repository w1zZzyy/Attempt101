#include "bus.hpp"
#include <mutex>
#include <stdexcept>

namespace Scene::Shared 
{

void Bus::Launch(size_t threads) 
{
    if(threads < 1) {
        throw std::runtime_error("at least 1 thread");
    }

    workers.resize(threads);
    flag = Mode::ProcessingQueries;

    for(auto& worker : workers) {
        worker = std::thread(
            [this]() 
            {
                while(flag != Mode::Stopping) 
                {
                    std::unique_lock<std::mutex> ul(mtx);
                    cv.wait(ul, [this](){return 
                        flag == Mode::Stopping || can_proceed();
                    });

                    if(flag == Mode::Stopping)
                        break;

                    auto [key, event] = waiting.front();
                    waiting.pop();

                    ul.unlock();

                    for(auto& handler : subscribers[key]) {
                        handler(*event);
                    }
                }
            }
        );
    }
}

void Bus::SwitchMode(Mode newMode) 
{
    flag = newMode;
    if(can_proceed()) {
        cv.notify_one();
    }
}

bool Bus::can_proceed() const noexcept
{
    return !waiting.empty() && flag == Mode::ProcessingQueries;
}

void Bus::Clear() {
    std::lock_guard<std::mutex> lg(mtx);
    subscribers.clear();
    while(!waiting.empty())
        waiting.pop();
}

Bus::~Bus() 
{
    flag = Mode::Stopping;
    cv.notify_all();

    for(auto& t : workers) {
        if(t.joinable())
            t.join();
    }
}



}