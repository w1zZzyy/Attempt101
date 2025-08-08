#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include <functional>
#include <queue>
#include "event_type.hpp"

namespace event
{


class Bus
{
public:

    template<EventType ET>
    using Handler = std::function<void(const ET&)>;

    template<EventType ET>
    void subscribe(const Handler<ET>& handler);

    template<EventType ET>
    void publish(const ET& event) const;

    template<EventType ET, typename... Args>
    void enqueue(Args&& ... args);

    void publish_all() {
        while(!event_queue.empty()) {
            publish(*event_queue.front());
            event_queue.pop();
        }
    }

private:

    std::unordered_map<std::type_index, std::vector<Handler<IEvent>>> subscribers;
    std::queue<EventPtr> event_queue;

};

template <EventType ET>
inline void Bus::subscribe(const Handler<ET> &handler)
{
    auto key = typeid(ET);
    subscribers[key].emplace_back(
        [handler](const IEvent& event) {
            handler(static_cast<const ET&>(event));
        };
    );
}

template <EventType ET>
inline void Bus::publish(const ET &event) const
{
    auto key = typeid(ET);
    auto it = subscribers.find(key);
    if(it != subscribers.end()) {
        for(const Handler<ET>& handler : it->second) {
            handler(event);
        }
    }
}

template <EventType ET, typename... Args>
inline void Bus::enqueue(Args &&...args)
{
    event_queue.emplace(CreateEvent<ET>(std::forward<Args>(args...)));
}

}