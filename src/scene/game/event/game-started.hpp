#pragma once

#include "core/logic/defs.hpp"
#include "core/logic/movelist.hpp"
#include "scene/model/event.hpp"
#include "core/logic/position.hpp"

namespace Scene::Game::Event 
{

struct GameStarted : public Model::IEvent {
    GameStarted(
        Core::Logic::Color player,
        const Core::Logic::PositionDM& pos, 
        const Core::Logic::MoveList& moves
    ) noexcept :
        player(player),
        pos(pos), 
        moves(moves)
    {}
    Core::Logic::Color player;
    const Core::Logic::PositionDM& pos;
    const Core::Logic::MoveList& moves;
};

}