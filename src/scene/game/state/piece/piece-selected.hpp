#pragma once 

#include "core/logic/square.hpp"
#include "scene/game/event/mouse.hpp"
#include "ui/renderer/pieces.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Piece 
{

class InProgressIdle;
class PieceGrabbed;

class PieceSelected : public InProgress<PieceSelected, UI::Renderer::Pieces> {
public:

    using Object = UI::Renderer::Pieces;

    PieceSelected(
        Core::Logic::Color player, 
        const Core::Logic::PositionDM &pos, 
        const Core::Logic::MoveList &moves,
        Core::Logic::Square on
    ) noexcept;

    Model::NextState<InProgressIdle, PieceGrabbed> HandleEventImpl(const Event::MousePressed&);
    Model::NextState<InProgressIdle> HandleEventImpl(const Event::GameUpdated&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return 
            std::is_same_v<T, Event::MousePressed> || 
            std::is_same_v<T, Event::GameUpdated>;
    }

private:

    Core::Logic::Square on;

};

}