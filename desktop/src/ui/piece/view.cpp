#include "view.hpp"

#include "resources/textures_manager/view.hpp"

namespace ui
{

PieceSprite::PieceSprite(game::logic::Color c, game::logic::Piece p) : 
    sprite(*resource::TextureManager::Get(c, p)), 
    color(c),
    piece(p)
{}

PieceSprite::PieceSprite(PieceSprite&& ps) noexcept : 
    sprite(std::move(ps.sprite)),
    piece(ps.piece),
    color(ps.color)
{}

void PieceSprite::Render(sf::RenderWindow& window) {
    window.draw(sprite);
}

PieceSprite& PieceSprite::setSize(sf::Vector2f size) {
    sf::Vector2u currSize = sprite.getTexture().getSize();
    sprite.setScale({
        size.x / currSize.x,
        size.y / currSize.y
    });
    return *this;
}

PieceSprite& PieceSprite::setPos(sf::Vector2f pos) {
    sprite.setPosition(pos);
    return *this;
}

PieceSprite &PieceSprite::setOriginCenter()
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.getCenter());
    return *this;
}

void PieceSprite::replacePiece(game::logic::Piece newPiece)
{
    auto texture = resource::TextureManager::Get(color, newPiece);
    sprite.setTexture(*texture);
    piece = newPiece;
}

void PieceSprite::operator = (PieceSprite&& _entity) noexcept {
    sprite = std::move(_entity.sprite);
    piece = _entity.piece;
    color = _entity.color;
}


}