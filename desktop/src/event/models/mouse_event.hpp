#pragma once

#include <SFML/Graphics.hpp>
#include "event_type.hpp"

namespace event
{

struct MousePressedEvent : IEvent {
    MousePressedEvent(sf::Vector2f pos) : pos(pos) {}
    sf::Vector2f pos;
};
struct MouseMovedEvent : IEvent {
    MouseMovedEvent(sf::Vector2f pos) : pos(pos) {}
    sf::Vector2f pos;
};
struct MouseReleasedEvent : IEvent {};

}