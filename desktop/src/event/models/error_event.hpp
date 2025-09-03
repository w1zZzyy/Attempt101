#pragma once

#include "event_type.hpp"
#include "logic/square.hpp"

namespace event
{

struct IErrorEvent : IEvent {};
struct MoveErrorEvent : IErrorEvent 
{
    enum Type {
        NotFound, 
        Ambiguous
    } type;
    MoveErrorEvent(
        Type type, 
        game::logic::Square from, 
        game::logic::Square targ)
        : type(type), from(from), targ(targ) 
    {}
    game::logic::Square from, targ;
};

}