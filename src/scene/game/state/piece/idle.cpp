#include "idle.hpp"

#include "core/logic/square.hpp"
#include "core/logic/defs.hpp"
#include "in-progress.hpp"

namespace Scene::Game::State::Piece
{

using Object = UI::Renderer::Pieces;

Model::NextState<InProgressIdle> Idle::HandleEventImpl(const Event::GameStarted& event)
{
    using namespace Core::Logic;

    for(Square sqr = Square::Start(); sqr.isValid(); ++sqr) {
        if(auto piece = event.pos.GetPiece(sqr); piece.isValid()) {
            Color color = event.pos.GetPieceColor(sqr);
            object->Append(color, piece, sqr);
        }
    }

    Model::NextState<InProgressIdle> next;
    next.Load<Object>(InProgressIdle{
        event.player, 
        event.pos, 
        event.moves
    });

    return next;
}

}