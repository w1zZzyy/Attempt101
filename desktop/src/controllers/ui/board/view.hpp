#pragma once

#include "ui/board/view.hpp"
#include "event/bus/view.hpp"
#include "ui/board/manager.hpp"
#include "ui/piece/manager.hpp"

namespace controller
{


class UIGameController 
{

    using Square = game::logic::Square;
    using Move = game::logic::Move;
    using Color = game::logic::Color;

public:

    using OnPieceSelected = std::function<std::vector<Move>(Square)>;

    UIGameController(event::Bus& bus);

    UIGameController& SetBoardView(Color);
    UIGameController& SetOrigin(const sf::Vector2f&);
    UIGameController& SetCellShape(const sf::Vector2f&);
    UIGameController& SetCellColor(Color, sf::Color);
    UIGameController& SetHighlightedCellColor(sf::Color);
    UIGameController& SetOnPieceSelected(OnPieceSelected&&) noexcept;

    void Render(sf::RenderWindow& window);

private:

    void SubscribeOnPositionChangedEvent();
    void SubscribeOnMousePressedEvent();
    void SetPieceManagerCallbacks();

private:

    event::Bus& bus;

    ui::PiecesManager PiecesManager;
    ui::BoardManager BoardManager;

    OnPieceSelected onPieceSelected;

};

}