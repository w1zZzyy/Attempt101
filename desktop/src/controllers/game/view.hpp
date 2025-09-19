#pragma once

#include "event/bus/view.hpp"
#include "logic.hpp"

namespace controller
{


class GameLogicController
{
public:

    GameLogicController(event::Bus& bus);
    void Init(const std::string& fen);
    const GameLogic& GetLogic() const noexcept {return logic;}

private:

    void SubscribeOnMoveAppeared();
    void HandleMove(game::logic::Move move) const;

private:

    event::Bus& bus;
    GameLogic logic;

};


}