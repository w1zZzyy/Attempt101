#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <concepts>
#include <type_traits>
#include <memory>

#include "logic/src/defs.hpp"
#include "logic/src/move.hpp"

namespace event
{


struct IEvent {
    virtual ~IEvent() = default;
};


// CELL

struct CellHighlightedEvent : IEvent { 
    CellHighlightedEvent(game::logic::Square s) : highlighted(s) {}
    game::logic::Square highlighted; 
};


// PROMOTION 

struct ShowPromotionDialog : IEvent {
    ShowPromotionDialog() = default;
};


// MOVE 

struct MoveEvent : IEvent {
    MoveEvent(
        game::logic::Square f, 
        game::logic::Square t, 
        std::optional<game::logic::MoveFlag> fl = std::nullopt
    ) : from(f), targ(t), flag(fl) {}
    game::logic::Square from, targ;
    std::optional<game::logic::MoveFlag> flag;
};


// PIECE 

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


template<typename T>
concept EventType = std::is_base_of_v<IEvent, T>;


using EventPtr = std::unique_ptr<IEvent>;

template<EventType ET, typename... Args>
EventPtr CreateEvent(Args&&... args) {
    return std::make_unique<ET>(
        std::forward<Args>(args)...
    );
};


}