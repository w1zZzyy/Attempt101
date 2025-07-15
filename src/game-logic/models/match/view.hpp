#pragma once

#include "../player/view.hpp"
#include "../../logic/src/position.hpp"

namespace game::model
{


class Match {
public:

    Match(PlayerPtr&& white, PlayerPtr&& black);

protected:

    boost::asio::awaitable<void> await_player(logic::Color side);

    PlayerPtr       players[logic::Color::Count()];
    logic::Position position;
};



}