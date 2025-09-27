#pragma once 

#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

namespace UI::Utils 
{

void AppendQuad(sf::Vector2f origin, sf::Vector2f size, sf::Color clr, sf::VertexArray& arr);

}