#include "square.hpp"

#include "bitboard.hpp"
#include "attack.hpp"
#include "defs.hpp"

#include <cmath>

using namespace game::logic;


static Bitboard BetweenSquares[SQUARE_COUNT][SQUARE_COUNT];  
static Bitboard SameLineSquares[SQUARE_COUNT][SQUARE_COUNT];


DirectionType DirFromToTarg(Square from, Square targ)
{
    int fromY = from / 8, fromX = from % 8;
    int targY = targ / 8, targX = targ % 8;

    return(  
        (fromY == targY) ? (fromX > targX) ? WEST  : EAST  :
        (fromX == targX) ? (fromY > targY) ? SOUTH : NORTH :
        (std::abs(fromX - targX) != std::abs(fromY - targY)) ? NO_DIRECTION :
        (fromX > targX) 
            ? (fromY > targY) 
                ? SOUTH_WEST 
                : NORTH_WEST
            : (fromY > targY)
                ? SOUTH_EAST
                : NORTH_EAST);
}


void SetupBetween()
{
    AttackParams params;

    for(Square from = Square::Start(); from < SQUARE_COUNT; ++from)
    {
        params.set_attacker(from);
        for(Square targ = Square::Start(); targ < SQUARE_COUNT; ++targ)
        {
            if (from == targ) 
                continue;

            params.set_dir(DirFromToTarg(from, targ));
            params.set_blockers(targ.bitboard());
            BetweenSquares[from][targ] = GetSlowAttack(QUEEN, params);
        }
    }
}


void SetupSameLine()
{
    AttackParams params; params.set_blockers(Bitboard::Null());

    for(Square from = Square::Start(); from < SQUARE_COUNT; ++from)
    {
        params.set_attacker(from);
        for(Square targ = Square::Start(); targ < SQUARE_COUNT; ++targ)
        {
            if (from == targ) 
                continue;

            DirectionType dir = DirFromToTarg(from, targ);
            if(dir == NO_DIRECTION)
                continue;

            params.set_dir(dir);
            Bitboard dir_attacks = GetSlowAttack(QUEEN, params);
            
            params.set_dir(DirectionType(-dir));
            Bitboard anti_dir_attacks = GetSlowAttack(QUEEN, params);

            SameLineSquares[from][targ] = dir_attacks | anti_dir_attacks | from.bitboard();
        }
    }
}


void game::logic::Square::Setup()
{
    SetupBetween();
    SetupSameLine();
}

Bitboard game::logic::Square::bitboard() const noexcept
{
    return Bitboard(1ULL << index);
}


Bitboard game::logic::between(Square sq1, Square sq2)
{
    return BetweenSquares[sq1][sq2];
}

Bitboard game::logic::line_bb(Square sq1, Square sq2)
{
    return SameLineSquares[sq1][sq2];
}


std::ostream& operator << (std::ostream& out, const game::logic::Square& sqr)
{
    out << sqr.to_string();
    return out;
}


bool game::logic::same_file(Square sq1, Square sq2) noexcept {return sq1 % 8 == sq2 % 8;}
bool game::logic::same_rank(Square sq1, Square sq2) noexcept {return sq1 / 8 == sq2 / 8;}