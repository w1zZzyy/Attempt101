#pragma once 

#include "scene/model/event.hpp"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <thread>

namespace Scene::Shared 
{

class Bus {
public:

    enum class Mode {
        AddingListeners, 
        ProcessingQueries,
        Stopping
    };

    ~Bus();

    void Clear();
    void Launch(size_t threads = 1);

    template<Model::EventType T>
    using Handler = std::function<void(const T&)>;

    template<Model::EventType T>
    void Subscribe(Handler<T>&& handler);

    template<Model::EventType T>
    void Publish(T&& event);

private:

    std::unordered_map<std::type_index, std::vector<Handler<Model::IEvent>>> subscribers;
    std::queue<std::pair<std::type_index, std::shared_ptr<Model::IEvent>>> waiting;
    std::vector<std::thread> workers;
    std::condition_variable cv;
    std::mutex mtx;
    std::atomic<Mode> flag = Mode::AddingListeners;

};

template<Model::EventType T>
void Bus::Subscribe(Handler<T> &&handler)
{
    if(flag != Mode::AddingListeners)
        throw std::runtime_error("cant add listeners in this mode");

    std::type_index key = typeid(T);
    subscribers[key].emplace_back(
        [handler](const Model::IEvent& event) {
            handler(static_cast<const T&>(event));
        }
    );
}

template<Model::EventType T>
void Bus::Publish(T &&event)
{
    {
        std::lock_guard<std::mutex> lg(mtx);
        waiting.push({typeid(T), std::make_shared<T>(std::move(event))});
    }
    cv.notify_one();
}

}