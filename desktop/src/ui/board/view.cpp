#include "view.hpp"

#include "../entity/view.hpp"

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

void BoardRenderer::Render(sf::RenderWindow &window) const
{
    using namespace game::logic;

    sf::RectangleShape rect_sqr(square_shape);
    rect_sqr.setSize(square_shape);

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        rect_sqr.setPosition(SquarePosition::GetPos(sqr));
        rect_sqr.setFillColor(square_clr[SquarePosition::GetSquareColor(sqr)]);
        window.draw(rect_sqr);
    }
}

}