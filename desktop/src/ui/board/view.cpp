#include "view.hpp"

#include "../models/entity.hpp"
#include "../models/square.hpp"

namespace ui
{

BoardRenderer &BoardRenderer::setSquareColor(
    game::logic::Color side, 
    sf::Color color
) noexcept {
    square_clr[side] = color;
    return *this;
}

BoardRenderer &BoardRenderer::setSquareSize(sf::Vector2f s) noexcept
{
    square_shape = s;
    return *this;
}

BoardRenderer &BoardRenderer::setHighlighColor(sf::Color color) noexcept
{
    highligh = color;
    return *this;
}

void BoardRenderer::Render(sf::RenderWindow &window) const
{
    sf::RectangleShape rect_sqr(square_shape);
    rect_sqr.setSize(square_shape);

    for(
        game::logic::Square sqr = game::logic::Square::Start(); 
        sqr <= game::logic::Square::End(); ++sqr
    ) {
        rect_sqr.setPosition(model::Square::GetPos(sqr));
        rect_sqr.setFillColor(square_clr[model::Square::GetSquareColor(sqr)]);
        window.draw(rect_sqr);
    }
}

}