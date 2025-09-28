#include "idle.hpp"

#include "core/logic/square.hpp"
#include "core/logic/defs.hpp"

namespace Scene::Game::State::Piece
{

std::optional<InProgress> Idle::HandleEventImpl(const Event::GameStarted& event)
{
    using namespace Core::Logic;
    for(Square sqr = Square::Start(); sqr.isValid(); ++sqr) {
        if(auto piece = event.pos.GetPiece(sqr); piece.isValid()) {
            Color color = event.pos.GetPieceColor(sqr);
            object->Append(color, piece, sqr);
        }
    }
    return InProgress{};
}

}