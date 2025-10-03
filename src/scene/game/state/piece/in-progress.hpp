#pragma once 

#include "scene/game/event/mouse.hpp"
#include "ui/renderer/pieces.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Piece
{

class PieceGrabbed;

class InProgressIdle : public InProgress<InProgressIdle, UI::Renderer::Pieces> {
public:

    using Object = UI::Renderer::Pieces;
    using InProgress<InProgressIdle, Object>::InProgress;

    Model::NextState<PieceGrabbed> HandleEventImpl(const Event::MousePressed&);
    Model::NoNextState<Object> HandleEventImpl(const Event::GameUpdated&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return 
            std::is_same_v<T, Event::MousePressed> || 
            std::is_same_v<T, Event::GameUpdated>;
    }
    
};

}