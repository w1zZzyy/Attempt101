#include "view.hpp"

namespace controller
{


GameManager::GameManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnMoveEvent();
}

void GameManager::Init(const std::string& fen)
{
    manager.Init(fen);

    using namespace game::logic;

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(Piece piece = manager.getPieceOn(sqr); piece.isValid()) 
        {
            Color clr = manager.getPieceClr(sqr);
            bus.publish<event::PieceAddedEvent>({piece, clr, sqr});
        }
    }

    //bus.publish_all();
}

void GameManager::SubscribeOnMoveEvent()
{
    bus.subscribe<event::MoveEvent>(
        [this](const event::MoveEvent& event) {
            auto optmove = manager.DoMove(event.from, event.targ, event.flag);
            if(!optmove) HandleMoveError(optmove.error());
            else HandleMove(optmove.value());
        }
    );
}

void GameManager::HandleMoveError(game::LogicException err) const
{
    switch (err)
    {
    case game::LogicException::PromotionFlagNeeded:
        bus.publish<event::ShowPromotionDialog>({});
        break;
    case game::LogicException::GameStatusError:
    case game::LogicException::MoveNotFound:
    default: break;
    }
}

void GameManager::HandleMove(game::logic::Move move)
{
    using namespace game::logic;

    MoveFlag flag = move.flag();
    Square from = move.from(), targ = move.targ();
    
    if(auto captured = manager.getCaptured(); captured.isValid()) {
        Square captured_on = flag == MoveFlag::EN_PASSANT_MF 
        ? where_passant(from, targ)
        : targ;

        bus.enqueue<event::PieceRemovedEvent>(captured_on);
    }

    switch (flag)
    {
    case MoveFlag::S_CASTLE_MF:
        bus.enqueue<event::PieceMovedEvent>(targ + EAST, from + EAST);
        break;
    case MoveFlag::L_CASTLE_MF:
        bus.enqueue<event::PieceMovedEvent>(targ + 2 * WEST, from + WEST);
        break;
    case MoveFlag::Q_PROMOTION_MF:
        bus.enqueue<event::PieceMovedEvent>(from, targ, QUEEN);
        break;
    case MoveFlag::K_PROMOTION_MF:
        bus.enqueue<event::PieceMovedEvent>(from, targ, KNIGHT);
        return;
    case MoveFlag::B_PROMOTION_MF:
        bus.enqueue<event::PieceMovedEvent>(from, targ, BISHOP);
        return;
    case MoveFlag::R_PROMOTION_MF:
        bus.enqueue<event::PieceMovedEvent>(from, targ, ROOK);
        return;
    default:
        break;
    }

    bus.enqueue<event::PieceMovedEvent>(from, targ);

    bus.publish_all();
}

}