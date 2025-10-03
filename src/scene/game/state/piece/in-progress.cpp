#include "in-progress.hpp"

#include "piece-grabbed.hpp"

namespace Scene::Game::State::Piece 
{

Model::NextState<PieceGrabbed> InProgressIdle::HandleEventImpl(const Event::MousePressed& event)
{
    if(!event.sqr)
        return {};

    if(!pos.GetPiece(*event.sqr).isValid())
        return {};

    const bool IsMyPiece = pos.GetPieceColor(*event.sqr) == player;
    auto& OptPiece = object->GetPiece(*event.sqr);
    assert(OptPiece.has_value());

    Model::NextState<PieceGrabbed> next;
    next.Load<Object>(PieceGrabbed{
        player, 
        pos, 
        moves, 
        OptPiece.value(),
        *event.sqr,
        IsMyPiece
    });
    return next;
}

}