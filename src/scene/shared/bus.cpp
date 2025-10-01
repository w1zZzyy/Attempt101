#include "bus.hpp"
#include <mutex>
#include <stdexcept>

namespace Scene::Shared 
{

Bus::Bus(size_t threads) 
{
    if(threads < 1) {
        throw std::runtime_error("at least 1 thread");
    }

    workers.resize(threads);
    stop = false;

    for(auto& worker : workers) {
        worker = std::thread(
            [this]() 
            {
                while(!stop) 
                {
                    std::unique_lock<std::mutex> ul(mtx);
                    cv.wait(ul, [this](){return stop || !waiting.empty();});

                    if(stop)
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

void Bus::Clear() {
    std::lock_guard<std::mutex> lg(mtx);
    subscribers.clear();
    while(!waiting.empty())
        waiting.pop();
}

Bus::~Bus() 
{
    stop = true;
    cv.notify_all();

    for(auto& t : workers) {
        if(t.joinable())
            t.join();
    }
}



}