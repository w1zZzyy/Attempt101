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
                if(auto newStateOpt = st.HandleEvent(event)) {
                    using NewState = std::decay_t<decltype(*newStateOpt)>;
                    newStateOpt->Init(object);
                    state = std::move(*newStateOpt);
                }
            }, 
            state
        );
    }
    
private:

    Object& object;
    std::variant<T...> state;

};

}