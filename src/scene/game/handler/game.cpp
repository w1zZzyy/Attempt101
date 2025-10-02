#include "game.hpp"
#include "core/logic/defs.hpp"
#include "scene/game/event/game-started.hpp"

namespace Scene::Game::Handler 
{

GameHandler::GameHandler(Shared::Bus& bus) : bus(bus) {}

void GameHandler::Init(const std::string& fen, Core::Logic::Color player) 
{
    pos.SetFen(fen);
    this->player = player;
}

void GameHandler::Start() 
{
    update();
    if(!result)
        bus.Publish<Event::GameStarted>({player, pos, moves});
}

void GameHandler::update() 
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

}