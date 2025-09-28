#pragma once

#include "state.hpp"

#include <stdexcept>
#include <variant>

namespace Scene::Model 
{

template<typename Object, StateType<Object>... T>
class Machine 
{
public:

    Machine(Object& object) noexcept : object(object), state(object) {}
    
    template<EventType Event>
    void HandleEvent(const Event& event) {
        std::visit(
            [&event, this](auto& st) {
                if(auto newStateOpt = st.HandleEvent(event)) {
                    using NewState = std::decay_t<decltype(*newStateOpt)>;
                    if(!std::holds_alternative<NewState>(state)) {
                        throw std::invalid_argument("NewState template not supported\n");
                    }
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