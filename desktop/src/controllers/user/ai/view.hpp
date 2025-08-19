#pragma once

#include "event/bus/view.hpp"
#include "engine/search.hpp"

namespace controller::user
{


class AIManager {
public:

    AIManager(event::Bus& bus);
    AIManager& SetSide(game::logic::Color _s) noexcept {side = _s; return *this;}
    AIManager& SetSearchDepth(int depth) {search.SetMaxDepth(depth); return *this;}
    AIManager& LaunchSearchWorker();

private: 

    void SubscribeOnPositionUpdates();

private:

    event::Bus& bus;
    game::logic::Color side;
    game::engine::Search search;

};


}