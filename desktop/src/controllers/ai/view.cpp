#include "view.hpp"

#include "event/models/position_event.hpp"
#include "logic/position.hpp"

namespace controller
{

AIController::AIController(event::Bus &bus) : bus(bus)
{
    SubscribeOnPositionUpdates();
}

AIController& AIController::SetPosition(const PositionDynamicMemory& pos) noexcept {
    engine.SetPosition(pos);
    return *this;
}

AIController& AIController::LaunchSearchWorker()
{
    engine.StartWorker({
        [this](game::logic::Move move) {
            bus.publish<event::PositionMoveAppearedEvent>(
                event::PositionMoveAppearedEvent{move}
            );
        }
    });
    return *this;
}

void AIController::SubscribeOnPositionUpdates()
{
    bus.subscribe<event::PositionChangedEvent>({
        [this](const event::PositionChangedEvent& event) {
            const PositionDynamicMemory& pos = *event.Position;
            if(pos.get_side() == side) 
                engine.FindBestMove();
        }
    });
}

}