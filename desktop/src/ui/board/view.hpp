#pragma once

#include <SFML/Graphics.hpp>
#include "logic/defs.hpp"

namespace ui
{


class BoardView {
public:

    static void SetOrigin(const sf::Vector2f& origin) noexcept {Origin = origin;}
    static void SetCellShape(const sf::Vector2f& shape) noexcept {Shape = shape;}
    static void SetPlayerView(game::logic::Color side) noexcept {PlayerView = side;}
    static void SetHighlightedColor(sf::Color clr) noexcept {Highlight = clr;}
    static void SetCellColor(game::logic::Color side, sf::Color clr) noexcept {Color[side] = clr;}

    static sf::Color GetColor(game::logic::Square sqr) noexcept;
    static sf::Color GetHighlighted() noexcept {return Highlight;}
    static sf::Vector2f GetShape() noexcept {return Shape;}

    static sf::RectangleShape CreateCell();

    static std::optional<game::logic::Square> ToSquare(sf::Vector2f pos);
    static sf::Vector2f ToCoordinates(game::logic::Square sqr);

private:

    inline static sf::Vector2f Origin;
    inline static sf::Vector2f Shape;
    inline static sf::Color Highlight;
    inline static sf::Color Color[game::logic::COLOR_COUNT];
    inline static game::logic::Color PlayerView;

};

}