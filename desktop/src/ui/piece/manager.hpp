#pragma once

#include <functional>
#include <expected>
#include "event/models/error_event.hpp"
#include "logic/move.hpp"
#include "view.hpp"

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

    enum class SelectionRights {
        WhitePieces, 
        BlackPieces,
        Sudo = WhitePieces | BlackPieces
    };

public:

    void Render(sf::RenderWindow& window);

    PiecesManager& SetPieceSelectedCallback(OnPieceSelected&&) noexcept;
    PiecesManager& SetPieceMovedCallback(OnPieceMoved&&) noexcept;
    PiecesManager& SetSelectionRights(int) noexcept;
    void SetSelectedMoves(std::vector<Move>&& moves);

    void AppendPiece(Color, Piece, Square);
    void RemovePiece(Square);
    void MovePiece(Square from, Square targ, std::optional<Piece> newPiece);

    void HandleMousePressedEvent(Square);
    void DropSelectedPiece();
    void TryToSelectPiece(Square sqr);

private:

    void TryToMove(Square from, Square targ);
    bool HasEnoughRights(Square sqr) const noexcept;
    struct SelectedPiece {
        Square sqr;
        std::vector<Move> moves;
    };

private:

    std::array<std::optional<PieceSprite>, game::logic::SQUARE_COUNT> pieces;

    std::optional<SelectedPiece> selected;
    SelectionRights rights;

    OnPieceSelected onPieceSelected;
    OnPieceMoved onPieceMoved;

};


}