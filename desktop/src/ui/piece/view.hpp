#pragma once

#include <SFML/Graphics.hpp>
#include "logic/defs.hpp"

namespace ui
{

class PieceSprite {
public:

    PieceSprite(game::logic::Color c, game::logic::Piece p);
    PieceSprite(PieceSprite&& _sprite) noexcept;
    void operator = (PieceSprite&& _entity) noexcept;

    void Render(sf::RenderWindow& window);
    PieceSprite& setSize(sf::Vector2f size);
    PieceSprite& setPos(sf::Vector2f pos);
    PieceSprite& setOriginCenter();
    void replacePiece(game::logic::Piece newPiece);

    game::logic::Color getColor() const noexcept {return color;}
    game::logic::Piece getPiece() const noexcept {return piece;}
    sf::Vector2f getPos() const noexcept {return sprite.getPosition();}

protected:

    sf::Sprite sprite;
    game::logic::Color color;
    game::logic::Piece piece;

};

}
