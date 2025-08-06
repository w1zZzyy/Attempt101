#pragma once

#include <SFML/Graphics.hpp>
#include "logic/src/defs.hpp"

namespace ui
{


class BoardRenderer
{
public:

    BoardRenderer& setSquareColor(game::logic::Color side, sf::Color color) noexcept;
    BoardRenderer& setSquareSize(sf::Vector2f size) noexcept;
    BoardRenderer& setHighlighColor(sf::Color color) noexcept;

    void Render(sf::RenderWindow& window) const;

private:

    sf::Color highligh;
    sf::Color square_clr[game::logic::COLOR_COUNT];
    sf::Vector2f square_shape;

};


}