#pragma once

#include <SFML/Graphics.hpp>
#include "logic/src/defs.hpp"

namespace desktop::ui
{


class BoardRenderer
{
public:

    BoardRenderer& setSquareColor(
        game::logic::Color side, 
        uint8_t r, uint8_t g, uint8_t b, uint8_t a
    ) noexcept;
    BoardRenderer& setSquareSize(float x, float y) noexcept;
    BoardRenderer& setStartPos(float x, float y) noexcept;

    void Render(sf::RenderWindow& window, game::logic::Color side_pov) const;

private:

    sf::Color square_clr[game::logic::COLOR_COUNT];
    sf::Vector2f square_shape;
    sf::Vector2f start_pos;

};


}