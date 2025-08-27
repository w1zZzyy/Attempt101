#pragma once

#include <SFML/Graphics.hpp>
#include "event/bus/view.hpp"

namespace controller
{


class InputManager
{
public:

    InputManager(event::Bus& bus);
    void HandleEvents(sf::RenderWindow& window);

private:

    event::Bus& bus;

};


}