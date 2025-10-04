#include "game.hpp"
#include "core/logic/defs.hpp"
#include "logic/move.hpp"
#include "scene/game/event/game-started.hpp"
#include "scene/game/event/game-updates.hpp"
#include "scene/game/event/promotion.hpp"

namespace Scene::Game::Handler 
{

GameHandler::GameHandler(Shared::Bus& bus) : bus(bus) {}

void GameHandler::Init(const std::string& fen, Core::Logic::Color player) 
{
    pos.SetFen(fen);
    this->player = player;

    OnUpdateAttempt();
}

void GameHandler::Start() 
{
    Update();
    if(!result)
        bus.Publish<Event::GameStarted>({player, pos, moves});
}

void GameHandler::Update() 
{
    pos.UpdateAttacks();
    moves.generate<Core::Logic::MoveGenType::All>(pos);

    if(moves.empty()) {
        const Core::Logic::Color side = pos.GetSide();
        result = "Game Ended! " + std::string(
            pos.IsCheck() 
                ? side == player 
                ? "You Lost, AI Won\n"
                : "You Won, AI Lost\n"
                : "Draw\n"
        );
        return;
    }

    if(pos.IsDraw()) {
        result = "Game Ended! Draw\n";
    }
}

void GameHandler::OnUpdateAttempt()
{
    using TEvent = Event::GameUpdateAttempted;
    bus.Subscribe<TEvent>(
        [this](const TEvent& event)
        {
            using namespace Core::Logic;

            struct MoveStatus {
                enum Type {
                    Found, 
                    NotFound,
                    Ambiguous
                } status;
                Core::Logic::Move move;
            } move_status;

            Square from = event.move.from();
            Square targ = event.move.targ();
            MoveFlag flag = event.move.flag();

            for(auto move : moves) 
            {
                if(move == event.move) {
                    move_status.status = MoveStatus::Found;
                    move_status.move = move;
                    break;
                }
                if(move.from() == from && move.targ() == targ) {
                    if(move_status.status == MoveStatus::Found && flag == DEFAULT_MF) {
                        move_status.status = MoveStatus::Ambiguous;
                        break;
                    }
                    move_status.status = MoveStatus::Found;
                    move_status.move = move;
                }
            }

            if(move_status.status == MoveStatus::Ambiguous) {
                bus.Publish<Event::Promotion>({from, targ});
                return;
            }
            if(move_status.status == MoveStatus::NotFound) {
                return;
            }

            pos.DoMove(move_status.move);
            Update();

            auto publish = [&](auto param) -> void {
                bus.Publish<Event::GameUpdated>({
                    from, targ, param
                });
            };

            switch (move_status.move.flag())
            {
            case EN_PASSANT_MF:
                publish(where_passant(from, targ));
                break;
            case L_CASTLE_MF:
                publish(std::make_pair(targ + 2 * WEST, from + WEST));
                break; 
            case S_CASTLE_MF:
                publish(std::make_pair(targ + EAST, from + EAST));
                break;   
            case Q_PROMOTION_MF:
            case K_PROMOTION_MF:
            case B_PROMOTION_MF:
            case R_PROMOTION_MF:
                publish(pos.GetPiece(targ));
                break;
            default:
                bus.Publish<Event::GameUpdated>({from, targ});
                break;
            }
        }
    );
}

}