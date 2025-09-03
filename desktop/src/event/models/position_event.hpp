#pragma once

#include "event_type.hpp"
#include "logic/move.hpp"
#include <vector>

namespace event 
{

struct PositionMoveAppearedEvent : IEvent 
{
    PositionMoveAppearedEvent(game::logic::Move move) : move(move) {}
    game::logic::Move move;
};

struct PositionChangedEvent : IEvent 
{
    using Square = game::logic::Square;
    using Color = game::logic::Color;
    using Piece = game::logic::Piece;

    struct Placement {
        Square square;
        Color color;
        Piece piece;
    };

    struct Move {
        Square from; 
        Square targ;
        std::optional<Piece> promotion;
    };
    
    std::vector<Placement> PiecesAdded;
    std::optional<Square> PieceRemoved;
    std::vector<Move> PieceMove;
    std::string fen;
    Color side;
};

}