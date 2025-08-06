#pragma once

#include "../models/entity.hpp"
#include "logic/src/defs.hpp"

namespace ui 
{


class PieceEntity : public model::Entity
{
public:

    explicit PieceEntity(sf::Texture& texture) : sprite(texture) {}

    void Render(sf::RenderWindow&) override;

    PieceEntity& setPiece(game::logic::Color c, game::logic::Piece p);
    Entity& setSize(sf::Vector2f) override;
    Entity& setPos(sf::Vector2f) override;

private:

    sf::Sprite sprite;

};


}