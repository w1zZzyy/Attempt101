#pragma once

#include "core/logic/defs.hpp"
#include "core/logic/movelist.hpp"
#include "core/logic/position.hpp"
#include "scene/model/state.hpp"

namespace Scene::Game::State
{

template<typename T, typename Object>
class InProgress : public Model::IState<T, Object> {
public:

    InProgress(
        Core::Logic::Color player,
        const Core::Logic::PositionDM& pos, 
        const Core::Logic::MoveList& moves
    ) noexcept : 
        player(player),
        pos(pos), 
        moves(moves) 
    {}

protected:

    Core::Logic::Color player;
    const Core::Logic::PositionDM& pos;
    const Core::Logic::MoveList& moves;
        
};

}