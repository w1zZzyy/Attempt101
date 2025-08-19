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

    void SubscribeOnInputBlockedEvent();
    void SubscribeOnInputAllowedEvent();

private:

    event::Bus& bus;
    bool isBlocked{true};

};


}