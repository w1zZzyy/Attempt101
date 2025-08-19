#pragma once

#include "event/bus/view.hpp"
#include "logic/position.hpp"

namespace controller::user
{


class HumanManager {
public:

    HumanManager(event::Bus& bus);
    HumanManager& SetSide(game::logic::Color _s) noexcept {side = _s; return *this;}

private: 

    void SubscribeOnPositionUpdates();

private:

    event::Bus& bus;
    game::logic::Color side;

};


}