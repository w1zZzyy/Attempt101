#include "move.hpp"
#include "square.hpp"

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


std::ostream& operator << (std::ostream& out, const game::logic::Move& m)
{
    Square from = m.from();
    Square targ = m.targ();
    MoveFlag flag = m.flag();

    out << from << targ;

    switch (flag) 
    {
        case game::logic::Q_PROMOTION_MF: out << 'q'; break;
        case game::logic::K_PROMOTION_MF: out << 'n'; break;
        case game::logic::R_PROMOTION_MF: out << 'r'; break;
        case game::logic::B_PROMOTION_MF: out << 'b'; break;
        default: break;
    }

    return  out;
}