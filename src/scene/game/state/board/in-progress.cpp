#include "in-progress.hpp"

#include "core/logic/defs.hpp"
#include "piece-selected.hpp"

#include <optional>

namespace Scene::Game::State::Board 
{

Model::NextState<PieceSelected> InProgressIdle::HandleEventImpl(const Event::MousePressed& event)
{
    if(!event.sqr)
        return {};

    Core::Logic::Piece piece = pos.GetPiece(*event.sqr);
    if(!piece.isValid())
        return {};

    Core::Logic::Color color = pos.GetPieceColor(*event.sqr);
    if(color != player)
        return {};

    object->SetSelected(*event.sqr);
    for(auto move : moves)
        if(move.from() == *event.sqr) 
            object->AppendValid(move.targ());

    Model::NextState<PieceSelected> next;
    next.Load<Object>(PieceSelected{player, pos, moves});
    return next;
}

Model::NoNextState<PieceSelected::Object> InProgressIdle::HandleEventImpl(const Event::GameUpdated& event)
{
    object->SetMove({event.from, event.targ, Core::Logic::DEFAULT_MF});
    return {};
}

}