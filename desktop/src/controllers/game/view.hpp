#pragma once

#include "event/bus/view.hpp"
#include "manager/view.hpp"

namespace controller
{


class GameManager
{
public:

    GameManager(event::Bus& bus);
    void Init(const std::string& fen);

private:

    void SubscribeOnMoveEvent();
    void HandleMoveError(game::LogicException err) const;
    void HandleMove(game::logic::Move move);

private:

    event::Bus& bus;
    game::LogicManager manager;

};


}