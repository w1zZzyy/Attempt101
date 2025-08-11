#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "logic/src/defs.hpp"

namespace ui
{


class BoardView {
public:

    void SetLeftBottomSquarePos(const sf::Vector2f& pos);
    void SetCellShape(const sf::Vector2f& shape);
    void SetCellColor(game::logic::Color side, sf::Color color);
    void SetHighlightedCellColor(sf::Color color);

    sf::Vector2f GetCellShape() const noexcept {return square_shape;}
    sf::Color GetColor(game::logic::Square sqr) const;
    virtual sf::Vector2f GetPosition(game::logic::Square sqr) const;

    sf::RectangleShape CreateCell() const;
    sf::RectangleShape CreateHighlightedCell() const;

protected:

    sf::Vector2f lb_sqr_pos;
    sf::Vector2f square_shape;
    sf::Color color[game::logic::COLOR_COUNT];
    sf::Color highlighted;

};

class ReversedBoardView : public BoardView {
public:
    sf::Vector2f GetPosition(game::logic::Square sqr) const override;
};

using BoardViewPtr = std::shared_ptr<BoardView>;

template<game::logic::ColorType T>
BoardViewPtr CreateBoardView() {
    if constexpr (T == game::logic::WHITE) {
        return std::make_shared<BoardView>();
    } else {
        return std::make_shared<ReversedBoardView>();
    }
}


class BoardRenderer {
public:

    void SetBoardView(BoardViewPtr& bvp) {board_view = bvp;}
    void Render(sf::RenderWindow& window) const;

private:

    BoardViewPtr board_view;

};


}