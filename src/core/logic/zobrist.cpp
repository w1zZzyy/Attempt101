#include "zobrist.hpp"

#include <random>

namespace Core::Logic
{

namespace
{


static U64 ZobristPieceTable[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];
static U64 ZobristCastleTable[CASTLE_COUNT];
static U64 ZobristPassantTable[SQUARE_COUNT];
static U64 ZobristSide;


}


void Zobrist::Setup()
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

Zobrist &Zobrist::updateSquare(Color c, Piece p, Square s) noexcept
{
    hash ^= ZobristPieceTable[c][p][s];
	return *this;
}

Zobrist &Zobrist::updateCastle(Castle cr) noexcept
{
    hash ^= ZobristCastleTable[cr];
	return *this;
}

Zobrist &Zobrist::updateEnPassant(Square sqr) noexcept
{
    hash ^= ZobristPassantTable[sqr];
	return *this;
}

Zobrist &Zobrist::updateSide()
{
    hash ^= ZobristSide;
	return *this;
}

}