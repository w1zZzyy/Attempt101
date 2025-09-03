#pragma once

#include "view.hpp"

namespace ui
{

class BoardManager {
public:

    void Render(sf::RenderWindow& window) const;
    void AddHighlighted(game::logic::Square sqr) {highlighted.push_back(sqr);}
    void CleanHighlighted() noexcept {highlighted.clear();}

private:

    void RenderCommonSquares(sf::RenderWindow& window) const;
    void RenderHighlightedSquares(sf::RenderWindow& window) const;

private:

    std::vector<game::logic::Square> highlighted;

};

}