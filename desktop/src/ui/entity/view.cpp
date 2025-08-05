#include "view.hpp"

namespace ui
{

sf::Vector2f SquarePosition::pos[game::logic::SQUARE_COUNT];

void SquarePosition::Init(
    game::logic::Color side_pov,
    sf::Vector2f left_bottom, 
    sf::Vector2f square_shape
) {
    assert(side_pov.isValid());

    using namespace game::logic;

    sf::Vector2f curr_pos = left_bottom;

    int from_y = side_pov.is(WHITE) ? 0 : 7;
    int targ_y = side_pov.is(WHITE) ? 8 : -1;
    int dy = side_pov.is(WHITE) ? 1 : -1;

    for(int y = from_y; y != targ_y; y += dy) 
    {
        for(int x = 0; x < 8; ++x) 
        {
            Square sqr(y * 8 + x);
            pos[sqr] = curr_pos;

            curr_pos.x += square_shape.x;
        }

        curr_pos.x = left_bottom.x;
        curr_pos.y -= square_shape.y;
    }
}

game::logic::Color SquarePosition::GetSquareColor(game::logic::Square sqr) noexcept
{
    assert(sqr.isValid());
    if(((sqr / 8) + (sqr % 8)) % 2 == 0) 
        return game::logic::BLACK;
    return game::logic::WHITE;
}
}