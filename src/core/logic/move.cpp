#include "move.hpp"
#include "square.hpp"

namespace Core::Logic
{

namespace offset
{

constexpr U8 targ = 6;
constexpr U8 flag = 12;
constexpr U8 mask = 63;


}


Move::Move(Square from, Square targ, MoveFlag flag)
{
    data = from | (targ << offset::targ) | (flag << offset::flag);
}

Square Move::from() const
{
    return Square::ToSquare(data & offset::mask);
}

Square Move::targ() const
{
    return Square::ToSquare(
        (data >> offset::targ) & offset::mask
    );
}

MoveFlag Move::flag() const
{
    return static_cast<MoveFlag>(data >> offset::flag);
}


std::string Move::to_string() const
{
    std::string res = from().to_string() + targ().to_string();

    switch (flag()) 
    {
        case Q_PROMOTION_MF: res += 'q'; break;
        case K_PROMOTION_MF: res += 'n'; break;
        case R_PROMOTION_MF: res += 'r'; break;
        case B_PROMOTION_MF: res += 'b'; break;
        default: break;
    }

    return  res;
}


std::ostream& operator << (std::ostream& out, const Move& m)
{
    out << m.to_string();
    return out;
}

}