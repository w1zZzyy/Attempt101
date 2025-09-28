#pragma once

#include "event.hpp"

#include <concepts>

namespace Scene::Model 
{

template<typename T, typename Object>
class IState {
public:

    IState() noexcept {object = nullptr;}
    IState(const IState& st) noexcept : object(st.object) {}
    IState(Object& object) noexcept : object(&object) {}

    void Init(Object& object) noexcept {
        this->object = &object;
    }

    template<EventType Event>
    auto HandleEvent(const Event& event) {
        assert(object);
        return static_cast<T*>(this)->HandleEventImpl(event);
    }

protected:

    Object* object;

};

template<typename T, typename Object>
concept StateType = std::derived_from<T, IState<T, Object>>;

}