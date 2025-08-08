#pragma once

#include "event/bus/view.hpp"
#include "manager/view.hpp"

namespace component
{


class GameManager
{
public:

    GameManager(event::Bus& bus);
    void Init(const std::string& fen);

private:

    void SubscribeOnMoveEvent();

private:

    event::Bus& bus;
    game::LogicManager manager;

};


}