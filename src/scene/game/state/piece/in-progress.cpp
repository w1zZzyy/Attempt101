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

Model::NoNextState<InProgressIdle::Object> InProgressIdle::HandleEventImpl(const Event::GameUpdated &event)
{
    if(event.flag) 
    {
        auto* event_ptr = &event.flag.value();

        if(auto promotion = std::get_if<Event::GameUpdated::Promotion>(event_ptr)) 
        {
            const auto clr = pos.GetPieceColor(event.targ);
            object->Replace(*promotion, clr, event.from);
        }

        else if(auto castle = std::get_if<Event::GameUpdated::RookCastle>(event_ptr)) 
        {
            object->Move(castle->first, castle->second);
        }

        else if(auto passant = std::get_if<Event::GameUpdated::Passant>(event_ptr))
        {
            object->Reset(*passant);
        }
    }

    object->Move(event.from, event.targ);

    return {};
}

}