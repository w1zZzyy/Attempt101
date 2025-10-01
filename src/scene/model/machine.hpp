#pragma once

#include "state.hpp"

#include <variant>

namespace Scene::Model 
{

template<typename Object, StateType<Object>... T>
class Machine 
{
public:

    Machine(Object& object) noexcept : object(object) {
        using FirstState = std::tuple_element_t<0, std::tuple<T...>>;
        state.template emplace<FirstState>(object);
    }
    
    template<EventType Event>
    void HandleEvent(const Event& event) {
        std::visit(
            [&event, this](auto& st) {
                NextState<Object> next = st.HandleEvent(event);
                next.template Dump<Object>(state);
                state.Init(object);
            }, 
            state
        );
    }

    template<EventType TEvent>
    bool Supports() {
        bool res;
        std::visit(
            [&res](auto& st) 
            {res = st.template Supports<TEvent>();}, 
            state
        );
        return res;
    }
    
private:

    Object& object;
    std::variant<T...> state;

};

}