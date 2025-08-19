#include "view.hpp"

#include <iostream>

namespace controller::user 
{

AIManager::AIManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnPositionUpdates();
}

AIManager& AIManager::LaunchSearchWorker()
{
    search.StartSearchWorker({
        [this](game::engine::Search::RootMove best) {
            bus.publish<event::MoveEvent>(event::MoveEvent{
                best.move.from(), 
                best.move.targ(), 
                best.move.flag()
            });
        }
    });
    return *this;
}

void AIManager::SubscribeOnPositionUpdates()
{
    bus.subscribe<event::PositionWasUpdated>({
        [this](const event::PositionWasUpdated& event) {
            if(event.new_side == side) 
                search.FindBestMove(event.new_fen);
        }
    });
}

}