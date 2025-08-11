#pragma once

#include "ui/board/view.hpp"
#include "ui/piece/view.hpp"
#include "event/bus/view.hpp"

#include <array>
#include <optional>

namespace controller
{


class UIBoardManager {
public:

    UIBoardManager(event::Bus& bus);

    UIBoardManager& SetBoardView(game::logic::Color side);
    UIBoardManager& SetLeftBottomSquarePos(const sf::Vector2f& pos);
    UIBoardManager& SetCellShape(const sf::Vector2f& shape);
    UIBoardManager& SetCellColor(game::logic::Color side, sf::Color color);
    UIBoardManager& SetHighlightedCellColor(sf::Color color);

    void Render(sf::RenderWindow& window);

private:

    void SubscribeOnPieceAddedEvent();
    void SubscribeOnPieceRemovedEvent();
    void SubscribeOnPieceMovedEvent();

private:

    event::Bus& bus;
    std::array<std::optional<ui::PieceEntity>, game::logic::SQUARE_COUNT> pieces;
    ui::BoardRenderer board;
    ui::BoardViewPtr board_view;

};

}