#include "piece-grabbed.hpp"

#include "in-progress.hpp"
#include "piece-selected.hpp"

namespace Scene::Game::State::Piece
{

PieceGrabbed::PieceGrabbed(
    Core::Logic::Color player, 
    const Core::Logic::PositionDM &pos, 
    const Core::Logic::MoveList &moves, 
    sf::Sprite &piece, 
    Core::Logic::Square on,
    bool canMove
) noexcept : 
    InProgress(player, pos, moves),
    piece(piece),
    on(on),
    canMove(canMove) 
{ idlePos = piece.getPosition(); }

Model::NoNextState<PieceGrabbed::Object> PieceGrabbed::HandleEventImpl(const Event::MouseMoved &event)
{
    piece.setPosition(event.pos);
    return {};
}

Model::NextState<InProgressIdle, PieceSelected> PieceGrabbed::HandleEventImpl(const Event::MouseReleased &event)
{
    piece.setPosition(idlePos);

    Model::NextState<InProgressIdle, PieceSelected> next;

    if(!canMove) {
        next.Load<Object>(InProgressIdle{player, pos, moves});
        return next;
    }

    if(!event.sqr) {
        next.Load<Object>(PieceSelected{player, pos, moves, on});
        return next;
    }

    if(MoveAttempted<InProgressIdle>(on, *event.sqr, next))
        return next;

    next.Load<Object>(PieceSelected{player, pos, moves, on});

    return next;
}

}