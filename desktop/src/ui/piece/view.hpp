#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "logic/defs.hpp"

namespace ui
{


class PieceEntity {
public:

    PieceEntity(game::logic::Color c, game::logic::Piece p);
    PieceEntity(PieceEntity&& pe) noexcept;
    PieceEntity(const PieceEntity&) = delete;

    void operator = (PieceEntity&& pe) noexcept;
    void operator = (const PieceEntity&) = delete;

    void Render(sf::RenderWindow& window);
    PieceEntity& setSize(sf::Vector2f size);
    PieceEntity& setPos(sf::Vector2f pos);
    void replacePiece(game::logic::Piece newPiece);

private:

    sf::Sprite sprite;
    game::logic::Color color;
    game::logic::Piece piece;

};


}
