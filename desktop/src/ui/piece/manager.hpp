#pragma once

#include <functional>
#include <expected>

#include "event/models/error_event.hpp"
#include "logic/move.hpp"
#include "view.hpp"
#include "selected.hpp"

namespace ui
{


class PiecesManager
{
private:

    using Square = game::logic::Square;
    using Piece = game::logic::Piece;
    using Color = game::logic::Color;
    using Move = game::logic::Move;

public:

    using OnPieceSelected = std::function<void(Square)>;
    using OnPieceMoved = std::function<void(
        std::expected<Move, event::MoveErrorEvent>
    )>;

public:

    void Render(sf::RenderWindow& window);

    PiecesManager& SetPieceSelectedCallback(OnPieceSelected&&) noexcept;
    PiecesManager& SetPieceMovedCallback(OnPieceMoved&&) noexcept;
    PiecesManager& SetSelectionRights(Color) noexcept;
    void SetSelectedMoves(Selected::Moves&& moves) noexcept {selected.SetMoves(std::move(moves));};
    void ResetSelected() noexcept;

    void AppendPiece(Color, Piece, Square);
    void RemovePiece(Square);
    void MovePiece(Square from, Square targ, std::optional<Piece> newPiece);

    void HandleMousePressedEvent(Square);
    void HandleMouseMovedEvent(sf::Vector2f);
    void HandleMouseReleasedEvent(sf::Vector2f);

private:

    bool TryToSelectPiece(Square sqr);
    bool TryToMove(Square from, Square targ);

private:

    std::array<std::optional<PieceSprite>, game::logic::SQUARE_COUNT> pieces;
    Selected selected;

    OnPieceSelected onPieceSelected;
    OnPieceMoved onPieceMoved;

};


}