#pragma once

#include "core/logic/defs.hpp"
#include "core/logic/movelist.hpp"
#include "core/logic/position.hpp"
#include "logic/move.hpp"
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

    template<Model::StateType<Object> OnUpdate>
    Model::NextState<OnUpdate> HandleGameUpdate(const Event::GameUpdated&);

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
            using namespace Core::Logic;
            next.template Load<Object>(OnMove{player, pos, moves});
            this->bus->template Publish<Event::GameUpdateAttempted>(Move{from, targ, DEFAULT_MF});
            return true;
        }
    }
    return false;
}

template <typename T, typename Object>
template <Model::StateType<Object> OnUpdate>
inline Model::NextState<OnUpdate>
InProgress<T, Object>::HandleGameUpdate(const Event::GameUpdated &event) 
{
    OnUpdate st(player, pos, moves);
    st.Init(*this->object, *this->bus);
    st.HandleEventImpl(event);

    Model::NextState<OnUpdate> next;
    next.template Load<Object>(std::move(st));

    return next;
}

}