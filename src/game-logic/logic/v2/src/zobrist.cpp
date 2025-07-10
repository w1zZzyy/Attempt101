#include "zobrist.hpp"

#include <random>
#include "bitboard.hpp"

using namespace game::logic;

namespace
{


static Bitboard ZobristPieceTable[Color::Count()][Piece::Count()][Square::Count()];
static Bitboard ZobristCastleTable[Castle::Count()];
static Bitboard ZobristPassantTable[Square::Count()];
static Bitboard ZobristSide;


}


void game::logic::Zobrist::Setup()
{
    U64 seed = 5489u;
    std::mt19937_64 gen(seed);
	std::uniform_int_distribution<U64> dist(0, UINT64_MAX);

    for (Color c = WHITE; c.isValid(); c.next())
		for (Piece p = KING; p.isValid(); p.next())
			for (Square s = a1; s < Square::Count(); ++s)
				ZobristPieceTable[c][p][s] = dist(gen);

	for (int i = 0; i < Castle::Count(); ++i)
		ZobristCastleTable[i] = dist(gen);

	for (Square s = a1; s < Square::Count(); ++s)
		ZobristPassantTable[s] = dist(gen);

	ZobristSide = dist(gen);
}

Zobrist &game::logic::Zobrist::updateSquare(Color c, Piece p, Square s)
{
    hash ^= ZobristPieceTable[c][p][s];
}

Zobrist &game::logic::Zobrist::updateCastle(Castle cr)
{
    hash ^= ZobristCastleTable[cr];
}

Zobrist &game::logic::Zobrist::updateEnPassant(Square sqr)
{
    hash ^= ZobristPassantTable[sqr];
}

Zobrist &game::logic::Zobrist::updateSide()
{
    hash ^= ZobristSide;
}
