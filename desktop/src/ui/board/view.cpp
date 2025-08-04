#include "view.hpp"

#include <ranges>

namespace desktop::ui
{

BoardRenderer &BoardRenderer::setSquareColor(
    game::logic::Color side, 
    uint8_t r, uint8_t g, uint8_t b, uint8_t a
) noexcept {
    square_clr[side] = sf::Color(r, g, b, a);
    return *this;
}

BoardRenderer &BoardRenderer::setSquareSize(float x, float y) noexcept
{
    square_shape = sf::Vector2f(x, y);
    return *this;
}

BoardRenderer &BoardRenderer::setStartPos(float x, float y) noexcept
{
    start_pos = sf::Vector2f(x, y);
    return *this;
}

void BoardRenderer::Render(sf::RenderWindow &window, game::logic::Color side_pov) const
{
    assert(side_pov == game::logic::WHITE || side_pov == game::logic::BLACK);

    sf::RectangleShape sqr(square_shape);

    sf::Vector2f curr_pos = start_pos;
    game::logic::Color curr_clr = (
        (side_pov == game::logic::WHITE) 
        ? game::logic::BLACK // a1 sqr is black
        : game::logic::WHITE // a8 sqr is white
    ); 

    for(int y = 0; y < 8; ++y) 
    {
        for(int x = 0; x < 8; ++x) 
        {
            sqr.setPosition(curr_pos);
            sqr.setFillColor(square_clr[curr_clr]);

            window.draw(sqr);

            curr_pos.x += square_shape.x;
            curr_clr.swap();
        }

        curr_clr.swap();

        curr_pos.x = start_pos.x;
        curr_pos.y -= square_shape.y;
    }
}

}