#include "cellsize.hpp"

namespace UI::Utils 
{

sf::Vector2f computeCellSize(
    const sf::Vector2f& size, 
    float padding
) {
    const sf::Vector2f boardSize = sf::Vector2f(
        size.x - 2 * padding,
        size.y - 2 * padding
    );
    return sf::Vector2f(
        boardSize.x / 8.0,
        boardSize.y / 8.0
    );
}

}