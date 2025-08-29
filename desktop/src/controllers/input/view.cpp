#include "view.hpp"
#include "event/models/mouse_event.hpp"

namespace controller
{


InputController::InputController(event::Bus &bus) : bus(bus) {}

void InputController::HandleEvents(sf::RenderWindow &window)
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
        else if (event->getIf<sf::Event::MouseButtonReleased>()) {
            bus.publish<event::MouseReleasedEvent>({});
        }
        else if(const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
            bus.publish<event::MouseMovedEvent>(sf::Vector2f{
                    (float)mouse->position.x, 
                    (float)mouse->position.y
            });
        }
    }
}

}