#pragma once

#include "game-logic/handler/game.hpp"

#include <expected>
#include <string_view>
#include <memory>

namespace match::model
{


class Player;
using PlayerPtr = std::shared_ptr<Player>;


class Player : std::enable_shared_from_this<Player>
{
public:

    std::expected<game::logic::Move, std::string> DoMove(
        std::string_view notation
    );

    PlayerPtr getPtr() {
        return shared_from_this();
    }

    game::handler::GamePtr getGame() noexcept {
        return game;
    }

private:

    game::handler::GamePtr game;
    game::logic::ColorType side;
    
};


}