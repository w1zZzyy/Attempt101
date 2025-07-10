#include "move.hpp"
#include "defs.hpp"

using namespace game::logic;

namespace offset
{

constexpr U8 targ = 6;
constexpr U8 flag = 12;
constexpr U8 mask = 63;


}


game::logic::Move::Move(Square from, Square targ, MoveFlag flag)
{
    data = from | (targ << offset::targ) | (flag << offset::flag);
}

Square game::logic::Move::from() const
{
    return Square::ToSquare(data & offset::mask);
}

Square game::logic::Move::targ() const
{
    return Square::ToSquare(
        (data >> offset::targ) & offset::mask
    );
}

MoveFlag game::logic::Move::flag() const
{
    return static_cast<MoveFlag>(data >> offset::flag);
}
