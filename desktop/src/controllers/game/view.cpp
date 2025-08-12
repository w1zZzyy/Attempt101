#include "view.hpp"

namespace controller
{


GameManager::GameManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnMoveEvent();
    SubscribeOnBoardClickedEvent();
}

void GameManager::Init(const std::string& fen)
{
    manager.Init(fen);

    using namespace game::logic;

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(Piece piece = manager.getPieceOn(sqr); piece.isValid()) 
        {
            Color clr = manager.getPieceClr(sqr);
            bus.enqueue<event::PieceAddedEvent>(piece, clr, sqr);
        }
    }

    bus.publish_all();
}

void GameManager::SubscribeOnMoveEvent()
{
    bus.subscribe<event::MoveEvent>(
        [this](const event::MoveEvent& event) {
            auto optmove = manager.DoMove(event.from, event.targ, event.flag);
            if(!optmove) HandleMoveError(event.targ, optmove.error());
            else HandleMove(optmove.value());
        }
    );
}

void GameManager::HandleMoveError(game::logic::Square attempted_targ, game::LogicException err) const
{
    switch (err)
    {
    case game::LogicException::PromotionFlagNeeded:
        bus.publish<event::ShowPromotionDialog>({});
        break;
    case game::LogicException::MoveNotFound:
        MaybePieceClick(attempted_targ);
        break;
    case game::LogicException::GameStatusError: break;
    default: break;
    }
}

void GameManager::HandleMove(game::logic::Move move)
{
    using namespace game::logic;

    const MoveFlag flag = move.flag();
    const Square from = move.from(), targ = move.targ();

    if(auto captured = manager.getCaptured(); captured.isValid()) 
        bus.enqueue<event::PieceRemovedEvent>(targ);
    else if(flag == EN_PASSANT_MF) 
        bus.enqueue<event::PieceRemovedEvent>(where_passant(from, targ));

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

void GameManager::SubscribeOnBoardClickedEvent()
{
    bus.subscribe<event::ClickedOnBoardEvent>(
        [this](const event::ClickedOnBoardEvent& event) {
            MaybePieceClick(event.square);
        }
    );
}

void GameManager::MaybePieceClick(game::logic::Square sqr) const
{
    if(auto piece = manager.getPieceOn(sqr); piece.isValid()) {
        auto moves = manager.MovesFrom(sqr);
        bus.publish<event::PieceSelectedEvent>({
            sqr, 
            std::move(moves)
        });
    }
}

}