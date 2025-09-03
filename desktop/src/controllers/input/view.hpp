#pragma once

#include <SFML/Graphics.hpp>
#include "event/bus/view.hpp"

namespace controller
{


class InputController
{
public:

    InputController(event::Bus& bus);
    void HandleEvents(sf::RenderWindow& window);

private:

    template<event::EventType ET>
    void PublishMouseEvent(sf::Vector2i mouse) {
        bus.publish<ET>(sf::Vector2f{
            (float)mouse.x, 
            (float)mouse.y
        });
    };


private:

    event::Bus& bus;

};


}