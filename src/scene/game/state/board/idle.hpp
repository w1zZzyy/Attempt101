#pragma once

#include "scene/game/event/game-started.hpp"
#include "scene/model/state.hpp"
#include "ui/renderer/board.hpp"

namespace Scene::Game::State::Board 
{

class InProgressIdle;

class Idle : public Model::IState<Idle, UI::Renderer::Board> {
public:

    Model::NextState<InProgressIdle> HandleEventImpl(const Event::GameStarted& event);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return std::is_same_v<T, Event::GameStarted>;
    }
};

}