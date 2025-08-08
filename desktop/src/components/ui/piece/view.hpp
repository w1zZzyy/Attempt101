#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "event/bus/view.hpp"

namespace component
{
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
    void setSize(sf::Vector2f size);
    void setPos(sf::Vector2f pos);
    void replacePiece(game::logic::Piece newPiece);

private:

    sf::Sprite sprite;
    game::logic::Color color;
    game::logic::Piece piece;

};


}


class PieceManager {
public:

    PieceManager(event::Bus& bus);
    void Render(sf::RenderWindow& window);
    void setPieceSize(sf::Vector2f size);

private:

    void SubscribeOnPieceAddedEvent();
    void SubscribeOnPieceRemovedEvent();
    void SubscribeOnPieceMoveEvent();

private:

    std::array<std::optional<ui::PieceEntity>, game::logic::SQUARE_COUNT> pieces;
    event::Bus& bus;

};


}