#pragma once

#include "scene/game/event/mouse.hpp"
#include "ui/renderer/board.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Board 
{

class InProgressIdle;

class PieceSelected : public InProgress<PieceSelected, UI::Renderer::Board>
{
public:

    using Object = UI::Renderer::Board;
    using InProgress<PieceSelected, Object>::InProgress;

    Model::NextState<InProgressIdle> HandleEventImpl(const Event::MousePressed&);
    Model::NoNextState<Object> HandleEventImpl(const Event::MouseMoved&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return 
            std::is_same_v<T, Event::MousePressed> || 
            std::is_same_v<T, Event::MouseMoved>;
    }
    
};

}