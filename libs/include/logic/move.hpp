#pragma once

#include "square.hpp"
#include "defs.hpp"
#include <ostream>

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

    Move(uint16_t d = 0) noexcept : data(d) {}
    Move(const Move& _m) noexcept : data(_m.data) {}
    Move(Square from, Square targ, MoveFlag flag);

    Move& operator = (const Move& _m) noexcept {data = _m.data; return *this;}
    void setMove(const Move& m) noexcept {data = m.data;}

    Square from() const;
    Square targ() const;
    MoveFlag flag() const;

    std::string to_string() const;

    constexpr operator uint16_t() const noexcept {return data;}

protected:

    uint16_t data;

};

class ExtMove : public Move 
{
public:

    void setScore(int s) noexcept { score = s; }
    bool operator < (const ExtMove& m) const noexcept {return score < m.score;}

private:

    int score = 0;

};

}


std::ostream& operator << (std::ostream& out, const game::logic::Move& m);