#include "view.hpp"

#include "../../resources/textures_manager/view.hpp"

namespace ui
{

void PieceEntity::Render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

PieceEntity &PieceEntity::setPiece(game::logic::Color c, game::logic::Piece p)
{
    auto texture = resource::TextureManager::Get(c, p);
    assert(texture);

    sprite.setTexture(*texture);

    return *this;
}

Entity &PieceEntity::setSize(sf::Vector2f newSize)
{
    auto currSize = sprite.getTexture().getSize();

    sprite.setScale({
        newSize.x / currSize.x,
        newSize.y / currSize.y
    });

    return *this;
}

Entity &PieceEntity::setPos(sf::Vector2f newPos)
{
    sprite.setPosition(newPos);
    return *this;
}

}

