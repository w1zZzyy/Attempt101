#pragma once

#include "event/bus/view.hpp"
#include "engine/engine.hpp"
#include "logic/position.hpp"

namespace controller
{


class AIController {
public:

    AIController(event::Bus& bus);
    AIController& SetSide(game::logic::Color _s) noexcept {side = _s; return *this;}
    AIController& SetPosition(const PositionDynamicMemory&) noexcept;
    AIController& SetSearchDepth(int depth) {engine.SetMaxDepth(depth); return *this;} 
    AIController& LaunchSearchWorker();

private: 

    void SubscribeOnPositionUpdates();

private:

    event::Bus& bus;
    game::logic::Color side;
    game::engine::Engine engine;

};


}