#include "view.hpp"

#include "resources/textures_manager/view.hpp"

namespace component
{
namespace ui
{

PieceEntity::PieceEntity(game::logic::Color c, game::logic::Piece p) : 
    sprite(*resource::TextureManager::Get(c, p)), 
    color(c),
    piece(p)
{}

PieceEntity::PieceEntity(PieceEntity &&pe) :
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

void PieceEntity::setSize(sf::Vector2f size) {
    auto currSize = sprite.getTexture().getSize();
    sprite.setScale({
        size.x / currSize.x,
        size.y / currSize.y
    });
}

void component::ui::PieceEntity::setPos(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void PieceEntity::replacePiece(game::logic::Piece newPiece)
{
    auto texture = resource::TextureManager::Get(color, newPiece);
    sprite.setTexture(*texture);
    piece = newPiece;
}

}


PieceManager::PieceManager(event::Bus &bus) : bus(bus)
{
    SubscribeOnPieceAddedEvent();
    SubscribeOnPieceRemovedEvent();
    SubscribeOnPieceMoveEvent();
}

void PieceManager::Render(sf::RenderWindow& window)
{
    for(auto& piece : pieces) {
        if(piece) 
            piece->Render(window);
    }
}

void PieceManager::setPieceSize(sf::Vector2f size)
{
    for(auto& piece : pieces) {
        if(piece) 
            piece->setSize(size);
    }
}

void PieceManager::SubscribeOnPieceAddedEvent()
{
    bus.subscribe<event::PieceAddedEvent>(
        [this](const event::PieceAddedEvent& event) {
            pieces[event.square].emplace(event.color, event.piece);
        }
    );
}

void PieceManager::SubscribeOnPieceRemovedEvent()
{
    bus.subscribe<event::PieceRemovedEvent>(
        [this](const event::PieceRemovedEvent& event) {
            pieces[event.captured_on].reset();
        }
    );
}

void PieceManager::SubscribeOnPieceMoveEvent()
{
    bus.subscribe<event::PieceMoveEvent>(
        [this](const event::PieceMoveEvent& event) {
            if(event.promotion)
                pieces[event.from]->replacePiece(*event.promotion);
            pieces[event.targ] = std::move(pieces[event.from]);
            pieces[event.from].reset();
        }
    );
}


}