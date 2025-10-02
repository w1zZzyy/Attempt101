#pragma once

#include "scene/model/event.hpp"
#include "scene/game/event/mouse.hpp"
#include "ui/renderer/board.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Board
{

class PieceSelected;

class InProgressIdle : public InProgress<InProgressIdle, UI::Renderer::Board> {
public:

    using Object = UI::Renderer::Board;
    using InProgress<InProgressIdle, Object>::InProgress;
    
    Model::NextState<PieceSelected> HandleEventImpl(const Event::MousePressed&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return std::is_same_v<T, Event::MousePressed>;
    }

};

}