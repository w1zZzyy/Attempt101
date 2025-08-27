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

private:

    void SubscribeOnMoveAppeared();

private:

    event::Bus& bus;
    GameLogic logic;

};


}