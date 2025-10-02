#pragma once

#include "event.hpp"

#include <concepts>
#include <cassert>
#include <type_traits>
#include <variant>
#include <optional>

namespace Scene::Model 
{

template<typename... TState>
class NextState;

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
        if constexpr (cast()->template SupportsImpl<Event>()) {
            assert(object);
            return cast()->HandleEventImpl(event);
        }
        else
            return NextState<T>();
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
        state.emplace(std::forward<T>(nextState)); 
    }

    template<typename Object, StateType<Object>... Dest>
    bool Dump(std::variant<Dest...>& dest) 
    {
        if constexpr(!IsValid<Object>() || !(is_convertible<TState, Dest...>() && ...))
            return false;

        else if(!state) 
            return false;
            
        else std::visit(
            [&dest](auto&& value) {
                using ValueType = std::decay_t<decltype(value)>;
                dest.template emplace<ValueType>(std::forward<decltype(value)>(value));
            }, 
            state.value()
        );

        return true;
    }

private:

    std::optional<std::variant<TState...>> state;
    
};


template<typename Object>
class NoState : public IState<NoState<Object>, Object> {
public:
    template<EventType T>
    Model::NextState<NoState> HandleEventImpl(const T&) {return {};}

    template<Model::EventType T>
    constexpr bool SupportsImpl() {return false;}
};

template<typename Object>
using NoNextState = NextState<NoState<Object>>;


}