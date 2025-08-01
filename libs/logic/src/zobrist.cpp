#include "zobrist.hpp"

#include <random>
#include "bitboard.hpp"

using namespace game::logic;

namespace
{


static Bitboard ZobristPieceTable[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];
static Bitboard ZobristCastleTable[CASTLE_COUNT];
static Bitboard ZobristPassantTable[SQUARE_COUNT];
static Bitboard ZobristSide;


}


void game::logic::Zobrist::Setup()
{
    U64 seed = 5489u;
    std::mt19937_64 gen(seed);
	std::uniform_int_distribution<U64> dist(0, UINT64_MAX);

    for (Color c = WHITE; c.isValid(); c.next())
		for (Piece p = KING; p.isValid(); p.next())
			for (Square s = a1; s < SQUARE_COUNT; ++s)
				ZobristPieceTable[c][p][s] = dist(gen);

	for (int i = 0; i < CASTLE_COUNT; ++i)
		ZobristCastleTable[i] = dist(gen);

	for (Square s = a1; s < SQUARE_COUNT; ++s)
		ZobristPassantTable[s] = dist(gen);

	ZobristSide = dist(gen);
}

Zobrist &game::logic::Zobrist::updateSquare(Color c, Piece p, Square s)
{
    hash ^= ZobristPieceTable[c][p][s];
	return *this;
}

Zobrist &game::logic::Zobrist::updateCastle(Castle cr)
{
    hash ^= ZobristCastleTable[cr];
	return *this;
}

Zobrist &game::logic::Zobrist::updateEnPassant(Square sqr)
{
    hash ^= ZobristPassantTable[sqr];
	return *this;
}

Zobrist &game::logic::Zobrist::updateSide()
{
    hash ^= ZobristSide;
	return *this;
}
