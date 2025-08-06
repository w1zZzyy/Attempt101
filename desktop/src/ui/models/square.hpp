#pragma once

#include "SFML/Graphics.hpp"
#include <logic/src/defs.hpp>

namespace ui::model
{

class Square
{
public:

    static void Init(
        game::logic::Color side_pov, 
        sf::Vector2f left_botom, 
        sf::Vector2f square_size
    );
    static sf::Vector2f GetPos(game::logic::Square sqr) noexcept {return pos[sqr];}
    static game::logic::Color GetSquareColor(game::logic::Square sqr) noexcept;

private:

    static sf::Vector2f pos[game::logic::SQUARE_COUNT];

};

}