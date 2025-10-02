#pragma once 

#include "scene/game/event/game-started.hpp"
#include "ui/renderer/pieces.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Piece
{

class InProgressIdle : public InProgress<InProgressIdle, UI::Renderer::Pieces> {
public:

    using Object = UI::Renderer::Pieces;
    using InProgress<InProgressIdle, Object>::InProgress;

    Model::NextState<InProgressIdle> HandleEventImpl(const Event::GameStarted&) {return {};};

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return std::is_same_v<T, Event::GameStarted>;
    }
    
};

}