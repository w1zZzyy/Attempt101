#pragma once 

#include "scene/game/event/mouse.hpp"
#include "scene/model/state.hpp"
#include "ui/renderer/promotion.hpp"

namespace Scene::Game::State::Promotion 
{

class Idle;

class InProgressIdle : public Model::IState<InProgressIdle, UI::Renderer::Promotion> {
public:

    using Object = UI::Renderer::Promotion;

    InProgressIdle& SetFrom(Core::Logic::Square from) noexcept;
    InProgressIdle& SetTarg(Core::Logic::Square targ) noexcept;

    Model::NoNextState<Object> HandleEventImpl(const Event::MouseMoved&);
    Model::NextState<Idle> HandleEventImpl(const Event::MousePressed&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return 
            std::is_same_v<T, Event::MouseMoved> || 
            std::is_same_v<T, Event::MousePressed>;
    }

private:

    Core::Logic::Square from, targ;

};

}