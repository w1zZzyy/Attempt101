#pragma once

#include "event/bus/view.hpp"
#include "engine/search.hpp"

namespace controller
{


class AIController {
public:

    AIController(event::Bus& bus);
    AIController& SetSide(game::logic::Color _s) noexcept {side = _s; return *this;}
    AIController& SetSearchDepth(int depth) {search.SetMaxDepth(depth); return *this;} 
    AIController& SetTableSize(size_t mb) {search.SetTableSize(mb); return *this;}
    AIController& LaunchSearchWorker();

private: 

    void SubscribeOnPositionUpdates();

private:

    event::Bus& bus;
    game::logic::Color side;
    game::engine::Search search;

};


}