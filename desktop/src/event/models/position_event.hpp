#pragma once

#include "event_type.hpp"
#include <logic/move.hpp>

namespace event 
{

struct PositionMoveAppearedEvent : IEvent 
{
    PositionMoveAppearedEvent(game::logic::Move move) : move(move) {}
    game::logic::Move move;
};

}