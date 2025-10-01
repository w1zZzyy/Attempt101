#pragma once

#include "event.hpp"

#include <concepts>
#include <cassert>
#include <type_traits>
#include <variant>
#include <optional>

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
        return cast()->HandleEventImpl(event);
    }

    template<EventType Event>
    constexpr bool Supports() {
        return cast()->template SupportsImpl<Event>();
    }

private:

    constexpr T* cast() {return static_cast<T*>(this);}

protected:

    Object* object;

};

template<typename T, typename Object>
concept StateType = std::derived_from<T, IState<T, Object>>;

template<typename... TState>
class NextState {

    template<typename From, typename... To>
    static constexpr bool is_convertible() {
        return (std::is_convertible_v<From, To> || ...);
    }

    template<typename Object>
    static constexpr bool IsValid() {
        return (StateType<TState, Object> && ...);
    }

public:

    template<typename Object, StateType<Object> T>
    void Load(T&& nextState) {
        static_assert(IsValid<Object>() && (std::is_same_v<T, TState> || ...), "type not supported");
        state = std::forward(nextState);
    }

    template<typename Object, StateType<Object>... Dest>
    void Dump(std::variant<Dest...>& dest) {
        static_assert(IsValid<Object>() && (is_convertible<TState, Dest...>() && ...), "cant convert");
        if(!state) return;
        std::visit(
            [&dest](auto&& value) {
                dest = std::move(value);
            }, 
            state.value()
        );
    }

private:

    std::optional<std::variant<TState...>> state;
    
};

}