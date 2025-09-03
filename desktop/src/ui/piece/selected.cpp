#include "selected.hpp"

namespace ui
{

void Selected::SetRights(game::logic::Color c) noexcept
{
    if(c.isValid()) 
        rights = static_cast<Rights>((int)c);
    else  
        rights = Rights::Sudo;
}

void Selected::SetMoves(Moves &&_moves) noexcept
{   
    assert(IsSettled());
    moves = std::move(_moves);
}

void Selected::SetSelected(game::logic::Square on) noexcept
{
    Reset();
    selected_on = on;
    status = Status::Grabbed;
}

void Selected::Drop() noexcept
{
    if(IsGrabbed()) 
        status = Status::Settled;
}

void Selected::Reset() noexcept
{
    status = Status::Idle;
    moves.clear();
}

bool Selected::IsSettled() const noexcept
{
    constexpr int __settled = (int)Status::Settled;
    return __settled & (int)status;
}

bool Selected::IsGrabbed() const noexcept
{
    return Status::Grabbed == status;
}

bool Selected::HasRights(game::logic::Color piece_color) const noexcept
{
    if(rights == Rights::Sudo)
        return true;
    return (int)rights == piece_color;
}

const Selected::Moves &Selected::GetMoves() const noexcept
{
    assert(IsSettled()); 
    return moves;
}

Selected::operator game::logic::Square() const noexcept 
{
    assert(IsSettled());
    return selected_on; 
}

} 