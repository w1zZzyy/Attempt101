#pragma once 

#include "SFML/System/Vector2.hpp"
#include "core/logic/defs.hpp"

namespace UI::Model 
{

struct Options {
    sf::Vector2f size;
    sf::Vector2f origin;
    Core::Logic::Color player;
    float padding;
};

}