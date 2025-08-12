#include "view.hpp"

namespace controller
{


InputManager::InputManager(event::Bus &bus) : bus(bus) {}

void InputManager::HandleEvents(sf::RenderWindow &window)
{
    while(auto event = window.pollEvent()) 
    {
        if(event->is<sf::Event::Closed>()) {
            window.close();
            break;
        }

        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                bus.publish<event::MousePressedEvent>(sf::Vector2f{
                    (float)mouseButtonPressed->position.x, 
                    (float)mouseButtonPressed->position.y
                });
            }
        }
    }
}

}