#pragma once

#include <type_traits>
#include <memory>

namespace event
{


struct IEvent {
    virtual ~IEvent() = default;
};

template<typename T>
concept EventType = std::is_base_of_v<IEvent, T>;


using EventPtr = std::unique_ptr<IEvent>;

template<EventType ET, typename... Args>
EventPtr CreateEvent(Args&&... args) {
    return std::make_unique<ET>(
        std::forward<Args>(args)...
    );
};


}