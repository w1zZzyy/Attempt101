#pragma once 

#include "core/logic/movelist.hpp"
#include "scene/game/event/mouse.hpp"
#include "ui/renderer/pieces.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Piece
{

class InProgressIdle;
class PieceSelected;

class PieceGrabbed : public InProgress<PieceGrabbed, UI::Renderer::Pieces> {
public:

    using Object = UI::Renderer::Pieces;

    PieceGrabbed(
        Core::Logic::Color player, 
        const Core::Logic::PositionDM& pos, 
        const Core::Logic::MoveList& moves, 
        sf::Sprite& piece, 
        Core::Logic::Square on,
        bool canMove
    ) noexcept;

    Model::NoNextState<Object> HandleEventImpl(const Event::MouseMoved&);
    Model::NextState<InProgressIdle, PieceSelected> HandleEventImpl(const Event::MouseReleased&);

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return 
            std::is_same_v<T, Event::MouseMoved> || 
            std::is_same_v<T, Event::MouseReleased>;
    }

private:

    sf::Sprite& piece;
    sf::Vector2f idlePos;
    Core::Logic::Square on;
    bool canMove;

};

}