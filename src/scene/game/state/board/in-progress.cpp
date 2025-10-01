#include "in-progress.hpp"

#include "core/logic/defs.hpp"
#include "piece-selected.hpp"

#include <optional>

namespace Scene::Game::State::Board 
{

using Object = UI::Renderer::Board;

Model::NextState<PieceSelected> InProgressIdle::HandleEventImpl(const Event::MousePressed& event)
{
    Model::NextState<PieceSelected> next;

    if(!event.sqr)
        return next;

    Core::Logic::Piece piece = pos.GetPiece(*event.sqr);
    if(!piece.isValid())
        return next;

    Core::Logic::Color color = pos.GetPieceColor(*event.sqr);
    if(color != player)
        return next;

    next.Load<Object>(PieceSelected{player, pos, moves});

    return next;
}

}