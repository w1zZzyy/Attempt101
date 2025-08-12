#include "view.hpp"

namespace ui
{

sf::RectangleShape BoardView::CreateCell() const
{
    sf::RectangleShape rect;
    rect.setSize(square_shape);
    return rect;
}

sf::RectangleShape BoardView::CreateHighlightedCell() const
{
    auto cell = CreateCell();
    cell.setFillColor(highlighted);
    return cell;
}


std::optional<game::logic::Square> BoardView::ToSquare(sf::Vector2f pos)
{
    sf::RectangleShape cell = CreateCell();

    using namespace game::logic;
    
    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        cell.setPosition(GetPosition(sqr));
        if(cell.getGlobalBounds().contains(pos)) return sqr;
    }

    return std::nullopt;
}

sf::Vector2f BoardView::GetPosition(game::logic::Square sqr) const
{
    int rank = sqr.rank();
    int file = sqr.file();

    return sf::Vector2f{
        lb_sqr_pos.x + square_shape.x * file,
        lb_sqr_pos.y - square_shape.y * rank,
    };
}

void BoardView::SetLeftBottomSquarePos(const sf::Vector2f& pos)
{
    lb_sqr_pos = pos;
}

void BoardView::SetCellShape(const sf::Vector2f &shape)
{
    square_shape = shape;
}

void BoardView::SetCellColor(game::logic::Color side, sf::Color c)
{
    color[side] = c;
}

void BoardView::SetHighlightedCellColor(sf::Color color)
{
    highlighted = color;
}

sf::Color BoardView::GetColor(game::logic::Square sqr) const
{
    int rank = sqr.rank();
    int file = sqr.file();
    return color[(rank + file) % 2 == 0];
}

sf::Vector2f ReversedBoardView::GetPosition(game::logic::Square sqr) const
{
    int rank = 7 - sqr.rank();
    int file = 7 - sqr.file();
    return BoardView::GetPosition({rank * 8 + file});
}

void BoardRenderer::Render(sf::RenderWindow &window) const
{
    assert(board_view);
    RenderCommonSquares(window);
    RenderHighlightedSquares(window);
}

void BoardRenderer::RenderCommonSquares(sf::RenderWindow &window) const
{
    sf::RectangleShape cell = board_view->CreateCell();

    using namespace game::logic;
    
    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        cell.setFillColor(board_view->GetColor(sqr));
        cell.setPosition(board_view->GetPosition(sqr));
        window.draw(cell);
    }
}
void BoardRenderer::RenderHighlightedSquares(sf::RenderWindow &window) const
{
    sf::RectangleShape cell = board_view->CreateHighlightedCell();

    using namespace game::logic;
    
    for(auto& sqr : highlighted) {
        cell.setPosition(board_view->GetPosition(sqr));
        window.draw(cell);
    }
}

}