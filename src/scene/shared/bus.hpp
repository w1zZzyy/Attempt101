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

/* 
1. сначала все компоненты должны подписаться на события
2. только после этого события стоит публиковать
P.S именно в таком порядке, что избежать лишнего копироваания слушателей
*/
class Bus {
public:

    Bus(size_t threads = 1);
    ~Bus();

    void Clear();

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
    bool stop;

};

template<Model::EventType T>
void Bus::Subscribe(Handler<T> &&handler)
{
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