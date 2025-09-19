#include "view.hpp"
#include "event/models/position_event.hpp"

namespace controller
{

GameLogicController::GameLogicController(event::Bus& bus) : bus(bus) {
    SubscribeOnMoveAppeared();
}

void GameLogicController::Init(const std::string& fen)
{
    logic.Init(fen);

    using namespace game::logic;
    const PositionDynamicMemory& pos = logic.GetPosition();

    event::PositionChangedEvent event;

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(Piece piece = pos.piece_on(sqr); piece.isValid()) 
        {
            Color clr = pos.piece_clr_on(sqr);
            event.PiecesAdded.push_back({sqr, clr, piece});
        }
    }

    event.Position = &pos;

    bus.publish<event::PositionChangedEvent>(event);
}

void GameLogicController::SubscribeOnMoveAppeared() 
{
    bus.subscribe<event::PositionMoveAppearedEvent>(
        [this](const event::PositionMoveAppearedEvent& event) {
            logic.DoMove(event.move);
            HandleMove(event.move);
        }
    );
}

void GameLogicController::HandleMove(game::logic::Move move) const
{
    using namespace game::logic;

    const PositionDynamicMemory& pos = logic.GetPosition();
    const MoveFlag flag = move.flag();
    const Square from = move.from(), targ = move.targ();

    event::PositionChangedEvent event;

    if(flag == EN_PASSANT_MF) 
        event.PieceRemoved.emplace(where_passant(from, targ));

    switch (flag)
    {
    case MoveFlag::S_CASTLE_MF:
        event.PieceMove.push_back({from, targ});
        event.PieceMove.push_back({targ + EAST, from + EAST});
        break;
    case MoveFlag::L_CASTLE_MF:
        event.PieceMove.push_back({from, targ});
        event.PieceMove.push_back({targ + 2 * WEST, from + WEST});
        break;
    case MoveFlag::Q_PROMOTION_MF:
    case MoveFlag::K_PROMOTION_MF:
    case MoveFlag::B_PROMOTION_MF:
    case MoveFlag::R_PROMOTION_MF:
        event.PieceMove.push_back({
            from, 
            targ, 
            pos.piece_on(targ)}
        );
        break;
    default:
        event.PieceMove.push_back({from, targ});
        break;
    }

    event.Position = &pos;

    bus.publish<event::PositionChangedEvent>(event);
}

}