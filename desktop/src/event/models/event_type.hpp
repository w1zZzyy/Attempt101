#pragma once

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

struct CellSelectedEvent : IEvent {
    CellSelectedEvent(game::logic::Square s) : selected(s) {} 
    game::logic::Square selected; 
};
struct CellHighlightedEvent : IEvent { 
    CellHighlightedEvent(game::logic::Square s) : highlighted(s) {}
    game::logic::Square highlighted; 
};

struct ShowPromotionDialog : IEvent {
    ShowPromotionDialog() = default;
};

struct MoveEvent : IEvent {
    MoveEvent(
        game::logic::Square f, 
        game::logic::Square t, 
        std::optional<game::logic::MoveFlag> fl = std::nullopt
    ) : from(f), targ(t), flag(fl) {}
    game::logic::Square from, targ;
    std::optional<game::logic::MoveFlag> flag;
};

struct PieceAddEvent : IEvent {
    PieceAddEvent(
        game::logic::Piece p,
        game::logic::Color c,
        game::logic::Square s, 
        float width, float height
    ) : 
        piece(p), color(c), square(s), 
        width(width), height(height) {}
    game::logic::Piece piece;
    game::logic::Color color;
    game::logic::Square square;
    float width, height;
};
struct PieceRemoveEvent : IEvent {
    PieceRemoveEvent(game::logic::Square rm_from) : square(rm_from) {}
    game::logic::Square square;
};
struct PieceReplaceEvent : IEvent {
    PieceReplaceEvent(
        game::logic::Square s, 
        game::logic::Piece new_p
    ) : square(s), new_piece(new_p) {}
    game::logic::Square square;
    game::logic::Piece new_piece;
};


template<typename T>
concept EventType = std::is_base_of_v<IEvent, T>;


}