#include "piece-selected.hpp"

#include "in-progress.hpp"
#include "piece-grabbed.hpp"

namespace Scene::Game::State::Piece 
{

PieceSelected::PieceSelected(
    Core::Logic::Color player, 
    const Core::Logic::PositionDM &pos, 
    const Core::Logic::MoveList &moves, 
    Core::Logic::Square on
) noexcept : 
    InProgress(player, pos, moves),
    on(on)
{}

Model::NextState<InProgressIdle, PieceGrabbed> PieceSelected::HandleEventImpl(const Event::MousePressed& event)
{
    Model::NextState<InProgressIdle, PieceGrabbed> next;

    if(!event.sqr || *event.sqr == on) {
        next.Load<Object>(InProgressIdle{player, pos, moves});
        return next;
    }

    if(pos.GetPiece(*event.sqr).isValid()) 
    {
        auto& piece = object->GetPiece(*event.sqr);
        assert(piece.has_value());
        next.Load<Object>(PieceGrabbed{
            player, pos, moves,
             *piece, *event.sqr, 
             pos.GetPieceColor(*event.sqr) == player
        });
        return next;
    }

    if(MoveAttempted<InProgressIdle>(on, *event.sqr, next))
        return next;

    return next;
}

}