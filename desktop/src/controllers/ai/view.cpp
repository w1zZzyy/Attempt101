#include "view.hpp"

#include "event/models/position_event.hpp"

namespace controller
{

AIController::AIController(event::Bus &bus) : bus(bus)
{
    SubscribeOnPositionUpdates();
}

AIController& AIController::LaunchSearchWorker()
{
    search.StartSearchWorker({
        [this](game::engine::Search::RootMove best) {
            bus.publish<event::PositionMoveAppearedEvent>(
            event::PositionMoveAppearedEvent{
                best.move
            });
        }
    });
    return *this;
}

void AIController::SubscribeOnPositionUpdates()
{
    bus.subscribe<event::PositionChangedEvent>({
        [this](const event::PositionChangedEvent& event) {
            if(event.side == side) 
                search.FindBestMove(event.fen);
        }
    });
}

}