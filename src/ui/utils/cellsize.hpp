#pragma once

#include "SFML/System/Vector2.hpp"

namespace UI::Utils 
{

sf::Vector2f computeCellSize(
    const sf::Vector2f& boardSize, 
    float padding
);

}