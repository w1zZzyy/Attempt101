#include "view.hpp"

namespace ui
{

sf::RectangleShape BoardView::CreateCell()
{
    sf::RectangleShape rect;
    rect.setSize(Shape);
    return rect;
}

sf::Color BoardView::GetColor(game::logic::Square sqr) noexcept {
    return Color[1 - (sqr.rank() + sqr.file()) % 2];
}

std::optional<game::logic::Square> BoardView::ToSquare(sf::Vector2f pos)
{
    sf::RectangleShape cell = CreateCell();

    using namespace game::logic;
    
    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        cell.setPosition(ToCoordinates(sqr));
        if(cell.getGlobalBounds().contains(pos)) return sqr;
    }

    return std::nullopt;
}

sf::Vector2f BoardView::ToCoordinates(game::logic::Square sqr)
{
    int rank = sqr.rank();
    int file = sqr.file();

    if(PlayerView.is(game::logic::BLACK)) {
        rank = 7 - rank;
        file = 7 - file;
    }

    return sf::Vector2f{
        Origin.x + Shape.x * file,
        Origin.y - Shape.y * rank,
    };
}


}