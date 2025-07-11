#pragma once

#include "square.hpp"

namespace game::logic
{


enum MoveFlag : U8
{
	DEFAULT_MF,
    S_CASTLE_MF,
    L_CASTLE_MF,
    Q_PROMOTION_MF,
    K_PROMOTION_MF,
    B_PROMOTION_MF,
    R_PROMOTION_MF,
    EN_PASSANT_MF,
    DOUBLE_MF
};


class Move
{
public:

    Move() noexcept : data(0) {}
    Move(const Move& _m) noexcept : data(_m.data) {}
    Move(Square from, Square targ, MoveFlag flag);

    Move& operator = (const Move& _m) noexcept {data = _m.data; return *this;}

    Square from() const;
    Square targ() const;
    MoveFlag flag() const;

private:

    unsigned short data;

};


}