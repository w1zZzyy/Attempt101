#include "square.hpp"

#include "bitboard.hpp"
#include "attack.hpp"

#include <cmath>

using namespace game::logic;


static Bitboard BetweenSquares[Square::Count()][Square::Count()];  
static Bitboard SameLineSquares[Square::Count()][Square::Count()];


void game::logic::Square::Setup()
{
    const auto& queen = AttackManager::GetPtr(QUEEN);
    AttackParams params;

    for(Square from = Square::Start(); from < Square::Count(); ++from)
    {
        params.set_attacker(from);
        for(Square targ = Square::Start(); targ < Square::Count(); ++targ)
        {
            if (from == targ) continue;

            int fromY = from / 8, fromX = from % 8;
			int targY = targ / 8, targX = targ % 8;

            DirectionType dir =  
				(fromY == targY) ? (fromX > targX) ? WEST  : EAST  :
				(fromX == targX) ? (fromY > targY) ? SOUTH : NORTH :
				(std::abs(fromX - targX) != std::abs(fromY - targY)) ? NO_DIRECTION :
				(fromX > targX) 
					? (fromY > targY) 
						? SOUTH_WEST 
						: NORTH_WEST
					: (fromY > targY)
						? SOUTH_EAST
						: NORTH_WEST
            ;

            // setting up between
            params.set_blockers(targ.bitboard());
            params.set_dir(dir);
            BetweenSquares[from][targ] = queen->GetSlowAttack(params);

            // setting up sameline
            params.set_blockers(0);
            SameLineSquares[from][targ] = 
                queen->GetSlowAttack(params) | 
                queen->GetSlowAttack(params.set_dir(DirectionType(-dir))) | 
                from.bitboard();
        }
    } 
}

Bitboard game::logic::Square::bitboard() const noexcept
{
    return Bitboard(1ULL << index);
}


Bitboard game::logic::between(Square sq1, Square sq2)
{
    return BetweenSquares[sq1][sq2];
}

Bitboard game::logic::same_line(Square sq1, Square sq2)
{
    return SameLineSquares[sq1][sq2];
}
