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

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left)
                PublishMouseEvent<event::MousePressedEvent>(mouse->position);
        }
        else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) 
            PublishMouseEvent<event::MouseReleasedEvent>(mouse->position);
        else if(const auto* mouse = event->getIf<sf::Event::MouseMoved>()) 
            PublishMouseEvent<event::MouseMovedEvent>(mouse->position);
    }
}

}