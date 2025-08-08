#include "view.hpp"

namespace component
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
            auto status = manager.getStatus();
            if(!optmove) {
                switch (optmove.error())
                {
                case game::LogicException::PromotionFlagNeeded:
                    bus.publish<event::ShowPromotionDialog>({});
                    break;
                case game::LogicException::GameStatusError:
                case game::LogicException::MoveNotFound:
                default: break;
                }
            }
            else 
            {
                using namespace game::logic;

                Move move = optmove.value();
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
                    bus.enqueue<event::PieceMoveEvent>(targ + EAST, from + EAST);
                    break;
                case MoveFlag::L_CASTLE_MF:
                    bus.enqueue<event::PieceMoveEvent>(targ + 2 * WEST, from + WEST);
                    break;
                case MoveFlag::Q_PROMOTION_MF:
                    bus.enqueue<event::PieceMoveEvent>(from, targ, QUEEN);
                    break;
                case MoveFlag::K_PROMOTION_MF:
                    bus.enqueue<event::PieceMoveEvent>(from, targ, KNIGHT);
                    return;
                case MoveFlag::B_PROMOTION_MF:
                    bus.enqueue<event::PieceMoveEvent>(from, targ, BISHOP);
                    return;
                case MoveFlag::R_PROMOTION_MF:
                    bus.enqueue<event::PieceMoveEvent>(from, targ, ROOK);
                    return;
                default:
                    break;
                }

                bus.enqueue<event::PieceMoveEvent>(from, targ);

                bus.publish_all();
            }
        }
    );
}

}