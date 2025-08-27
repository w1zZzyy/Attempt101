#pragma once

#include "event_type.hpp"
#include "logic/defs.hpp"

namespace event
{

struct PieceAddedEvent : IEvent {
    PieceAddedEvent(
        game::logic::Piece p,
        game::logic::Color c,
        game::logic::Square s
    ) : piece(p), color(c), square(s) {}
    game::logic::Piece piece;
    game::logic::Color color;
    game::logic::Square square;
};
struct PieceRemovedEvent : IEvent {
    PieceRemovedEvent(game::logic::Square rm_from) : captured_on(rm_from) {}
    game::logic::Square captured_on;
};
struct PieceMovedEvent : IEvent {
    PieceMovedEvent(
        game::logic::Square f, 
        game::logic::Square t, 
        std::optional<game::logic::Piece> prom = std::nullopt
    ) : from(f), targ(t), promotion(prom) {}
    game::logic::Square from, targ;
    std::optional<game::logic::Piece> promotion;
};

}