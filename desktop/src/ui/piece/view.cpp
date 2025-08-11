#include "view.hpp"

#include "resources/textures_manager/view.hpp"

namespace ui
{

PieceEntity::PieceEntity(game::logic::Color c, game::logic::Piece p) : 
    sprite(*resource::TextureManager::Get(c, p)), 
    color(c),
    piece(p)
{}

PieceEntity::PieceEntity(PieceEntity &&pe) noexcept :
    sprite(std::move(pe.sprite)),
    piece(pe.piece),
    color(pe.color)
{}

void PieceEntity::operator=(PieceEntity &&pe) noexcept
{
    sprite = std::move(pe.sprite);
    piece = pe.piece;
    color = pe.color;
}

void PieceEntity::Render(sf::RenderWindow& window) {
    window.draw(sprite);
}

PieceEntity& PieceEntity::setSize(sf::Vector2f size) {
    auto currSize = sprite.getTexture().getSize();
    sprite.setScale({
        size.x / currSize.x,
        size.y / currSize.y
    });
    return *this;
}

PieceEntity& PieceEntity::setPos(sf::Vector2f pos) {
    sprite.setPosition(pos);
    return *this;
}

void PieceEntity::replacePiece(game::logic::Piece newPiece)
{
    auto texture = resource::TextureManager::Get(color, newPiece);
    sprite.setTexture(*texture);
    piece = newPiece;
}


}