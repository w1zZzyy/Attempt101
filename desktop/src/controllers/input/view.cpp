#include "view.hpp"

namespace controller
{


InputManager::InputManager(event::Bus &bus) : bus(bus) 
{
    SubscribeOnInputBlockedEvent();
    SubscribeOnInputAllowedEvent();
}

void InputManager::HandleEvents(sf::RenderWindow &window)
{
    while(auto event = window.pollEvent()) 
    {
        if(event->is<sf::Event::Closed>()) {
            window.close();
            break;
        }

        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if(isBlocked) 
                continue;
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                bus.publish<event::MousePressedEvent>(sf::Vector2f{
                    (float)mouseButtonPressed->position.x, 
                    (float)mouseButtonPressed->position.y
                });
            }
        }
    }
}

void InputManager::SubscribeOnInputBlockedEvent()
{
    bus.subscribe<event::InputWasBlocked>({
        [this](const event::InputWasBlocked&) {
            this->isBlocked = true;
        }
    });
}

void InputManager::SubscribeOnInputAllowedEvent()
{
    bus.subscribe<event::InputWasAllowed>({
        [this](const event::InputWasAllowed&) {
            this->isBlocked = false;
        }
    });
}

}