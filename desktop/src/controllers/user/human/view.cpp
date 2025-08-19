#include "view.hpp"

namespace controller::user
{

HumanManager::HumanManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnPositionUpdates();
}

void HumanManager::SubscribeOnPositionUpdates()
{
    bus.subscribe<event::PositionWasUpdated>({
        [this](const event::PositionWasUpdated& event) {
            if(event.new_side != side) bus.publish<event::InputWasBlocked>({});
            else bus.publish<event::InputWasAllowed>({});
        }
    });
}

}