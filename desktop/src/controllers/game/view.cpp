#include "view.hpp"
#include "event/models/piece_event.hpp"

namespace controller
{

GameLogicController::GameLogicController(event::Bus& bus) : bus(bus) {
    SubscribeOnMoveAppeared();
}

void GameLogicController::Init(const std::string& fen)
{
    logic.Init(fen);

    using namespace game::logic;
    const PositionDynamicMemory& pos = logic;

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(Piece piece = pos.piece_on(sqr); piece.isValid()) 
        {
            Color clr = pos.piece_clr_on(sqr);
            bus.enqueue<event::PieceAddedEvent>(piece, clr, sqr);
        }
    }

    bus.publish_all();
}

void GameLogicController::SubscribeOnMoveAppeared() 
{
    //tututu
}

}