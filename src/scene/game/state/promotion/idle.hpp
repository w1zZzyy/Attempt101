#pragma once 

#include "scene/model/state.hpp"
#include "scene/game/event/promotion.hpp"
#include "ui/renderer/promotion.hpp"

namespace Scene::Game::State::Promotion 
{

class InProgressIdle;

class Idle : public Model::IState<Idle, UI::Renderer::Promotion> {
public:

    using Object = UI::Renderer::Promotion;

    Model::NextState<InProgressIdle> HandleEventImpl(const Event::Promotion& event);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return std::is_same_v<T, Event::Promotion>;
    }

};

}