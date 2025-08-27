#include "manager.hpp"

namespace ui
{

void BoardManager::Render(sf::RenderWindow &window) const
{
    RenderCommonSquares(window);
    RenderHighlightedSquares(window);
}

void BoardManager::RenderCommonSquares(sf::RenderWindow &window) const
{
    sf::RectangleShape cell = BoardView::CreateCell();

    using namespace game::logic;
    
    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        cell.setFillColor(BoardView::GetColor(sqr));
        cell.setPosition(BoardView::ToCoordinates(sqr));
        window.draw(cell);
    }
}
void BoardManager::RenderHighlightedSquares(sf::RenderWindow &window) const
{
    sf::RectangleShape cell = BoardView::CreateCell();
    cell.setFillColor(BoardView::GetHighlighted());

    using namespace game::logic;
    
    for(auto& sqr : highlighted) {
        cell.setPosition(BoardView::ToCoordinates(sqr));
        window.draw(cell);
    }
}

}