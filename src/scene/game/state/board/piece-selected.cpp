#include "piece-selected.hpp"

#include "in-progress.hpp"

namespace Scene::Game::State::Board 
{

Model::NextState<InProgressIdle> PieceSelected::HandleEventImpl(const Event::MousePressed &event)
{
    if(!event.sqr) 
        return {};

    const auto prevSelected = object->GetSelected();

    object->RemoveSelected();
    object->RemoveValid();

    if(
        !pos.GetPiece(*event.sqr).isValid() || 
        !pos.GetPieceColor(*event.sqr).isValid() || 
        prevSelected == event.sqr
    ) { 
        Model::NextState<InProgressIdle> next;
        next.Load<Object>(InProgressIdle{player, pos, moves}); 
        return next;
    }

    object->SetSelected(*event.sqr);
    for(auto move : moves)
        if(move.from() == *event.sqr) 
            object->AppendValid(move.targ());

    return {};
}

}