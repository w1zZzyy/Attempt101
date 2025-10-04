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
    object->RemoveHover();

    if(
        !pos.GetPiece(*event.sqr).isValid() || 
        pos.GetPieceColor(*event.sqr) != player || 
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

Model::NoNextState<PieceSelected::Object> PieceSelected::HandleEventImpl(const Event::MouseMoved &event)
{
    object->RemoveHover();
    
    if(!event.sqr)
        return {};

    const auto& valid = object->GetValid();
    for(auto& sqr : valid) {
        if(*event.sqr == sqr) {
            object->SetHover(sqr);
            break;
        }
    }

    return {};
}

Model::NextState<InProgressIdle> PieceSelected::HandleEventImpl(const Event::GameUpdated &event)
{
    auto next = ToInProgress();
    object->SetMove({event.from, event.targ, Core::Logic::DEFAULT_MF});
    return next;
}

Model::NextState<InProgressIdle> PieceSelected::HandleEventImpl(const Event::Promotion &)
{
    return ToInProgress();
}

Model::NextState<InProgressIdle> PieceSelected::ToInProgress()
{
    object->RemoveSelected();
    object->RemoveValid();
    object->RemoveHover();

    Model::NextState<InProgressIdle> next;
    next.Load<Object>(InProgressIdle{player, pos, moves});
    return next;
}

}