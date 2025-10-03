#pragma once

#include "state.hpp"
#include "scene/shared/bus.hpp"

#include <variant>

namespace Scene::Model 
{

template<typename Object, StateType<Object>... T>
class Machine 
{
public:

    Machine(Object& object, Shared::Bus& bus) noexcept : 
        object(object), 
        bus(bus)
    {
        using FirstState = std::tuple_element_t<0, std::tuple<T...>>;
        state.template emplace<FirstState>(object);
    }
    
    template<EventType Event>
    void HandleEvent(const Event& event) {
        std::visit(
            [&event, this](auto&& st) 
            {
                auto next = st.HandleEvent(event);
                if(next.template Dump<Object>(state))
                    std::visit([this](auto& st) {st.Init(object, bus);}, state);
            }, 
            state
        );
    }
    
protected:

    Object& object;
    Shared::Bus& bus;
    std::variant<T...> state;

};

}