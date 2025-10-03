#pragma once

#include "core/logic/defs.hpp"
#include "core/logic/movelist.hpp"
#include "core/logic/position.hpp"
#include "scene/model/state.hpp"
#include "scene/game/event/game-updates.hpp"

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

    template<Model::StateType<Object> OnMove, Model::StateType<Object>... TStates>
    bool MoveAttempted(Core::Logic::Square from, Core::Logic::Square targ, Model::NextState<TStates...>& next);

protected:

    Core::Logic::Color player;
    const Core::Logic::PositionDM& pos;
    const Core::Logic::MoveList& moves;
        
};

template <typename T, typename Object>
template <Model::StateType<Object> OnMove, Model::StateType<Object>... TStates>
inline bool InProgress<T, Object>::MoveAttempted(Core::Logic::Square from, Core::Logic::Square targ, Model::NextState<TStates...> &next)
{
    for(auto& move : moves) {
        if(move.from() == from && move.targ() == targ) {
            next.template Load<Object>(OnMove{player, pos, moves});
            this->bus->template Publish<Event::GameUpdateAttempted>({move});
            return true;
        }
    }
    return false;
}

}