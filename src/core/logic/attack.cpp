#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"

namespace Core::Logic
{

const int RookBits[] =
{
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int BishopBits[] =
{
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};


struct Magic
{
    Magic() = default;
    ~Magic() { delete[] magic; }

    Bitboard attacks;
    Bitboard* magic;
};


static Magic RookAttacksTable[SQUARE_COUNT];
static Magic BishopAttacksTable[SQUARE_COUNT];
static Bitboard KingAttacksTable[SQUARE_COUNT];
static Bitboard KnightAttacksTable[SQUARE_COUNT];
static Bitboard PawnAttacksTable[COLOR_COUNT][SQUARE_COUNT];


namespace Slow
{

template <DirectionType dir>
Bitboard getDirAttack(Square attacker, Bitboard blockers) noexcept
{
	Bitboard attacks;
    Bitboard piece = attacker.bitboard();
    Bitboard not_blocked = ~blockers;

    while(piece)
    {
        piece 	= 	step<dir>(piece);
        attacks |= 	piece;
        piece 	&= 	not_blocked;
    }

    return attacks;
}

template<DirectionType... Dirs>
Bitboard SlideAttacks(const AttackParams &p)
{
	assert(p.hasAttacker() && p.hasBlockers());
	
	Square attacker = p.get_attacker();
	Bitboard blocker = p.get_blockers();

	if(p.hasDir()) {
		std::optional<Bitboard> result;

		(void) std::initializer_list<int>
		{(
			(Dirs == p.get_dir()
            ? (result = getDirAttack<Dirs>(attacker, blocker), 0)
            : 0), ...
   		)};
		
		return result.value_or(Bitboard::Null());
	}

	return (getDirAttack<Dirs>(attacker, blocker) | ...);
}

Bitboard BishopAttack(const AttackParams &p)
{
	return SlideAttacks
	<
	SOUTH_EAST, SOUTH_WEST, 
	NORTH_EAST, NORTH_WEST
	>
	(p);
}

Bitboard RookAttack(const AttackParams &p)
{
	return SlideAttacks
	<
	EAST, WEST, 
	NORTH, SOUTH
	>
	(p);
}

Bitboard QueenAttack(const AttackParams& p)
{
	return BishopAttack(p) | RookAttack(p);
}

Bitboard KingAttack(const AttackParams& p)
{
	assert(p.hasAttacker());

	Bitboard king = p.get_attacker().bitboard();

	return 
		step<NORTH>(king) |
		step<EAST>(king) |
		step<WEST>(king) |
		step<SOUTH>(king) |
		step<NORTH_EAST>(king) |
		step<NORTH_WEST>(king) |
		step<SOUTH_EAST>(king) |
		step<SOUTH_WEST>(king);
}

Bitboard KnightAttack(const AttackParams& p)
{
	assert(p.hasAttacker());

	Bitboard attacks;
	Bitboard knight = p.get_attacker().bitboard();

	if (!(knight & FileType::FileH))
	{
		attacks |= (knight << 17) | (knight >> 15);
		if (!(knight & FileType::FileG))
			attacks |= (knight << 10) | (knight >> 6);
	}
	if (!(knight & FileType::FileA))
	{
		attacks |= (knight >> 17) | (knight << 15);
		if (!(knight & FileType::FileB))
			attacks |= (knight >> 10) | (knight << 6);
	}

	return attacks;
}

Bitboard PawnAttack(const AttackParams& p)
{
	assert(p.hasAttacker() && p.hasColor());

	Color clr = p.get_color();
	Bitboard pawn = p.get_attacker().bitboard();

	if(clr.is(WHITE)) {
		return 
			step<NORTH_WEST>(pawn) |
            step<NORTH_EAST>(pawn);
	} 
	if(clr.is(BLACK)) {
		return 
			step<SOUTH_WEST>(pawn) |
            step<SOUTH_EAST>(pawn);
	}

	return Bitboard::Null();
}

}


namespace Fast
{

inline Bitboard SlideAttacks(Magic* table, const AttackParams &p)
{
	assert(p.hasAttacker() && p.hasBlockers());

	Square attacker = p.get_attacker();

	Bitboard blocked = p.get_blockers() & table[attacker].attacks;
    int key = pext(blocked, table[attacker].attacks);

	return table[attacker].magic[key];
}
	
Bitboard BishopAttack(const AttackParams &p)
{
	return SlideAttacks(BishopAttacksTable, p);
}

Bitboard RookAttack(const AttackParams &p)
{
	return SlideAttacks(RookAttacksTable, p);
}

Bitboard QueenAttack(const AttackParams& p)
{
	return BishopAttack(p) | RookAttack(p);
}

Bitboard KingAttack(const AttackParams& p)
{
	assert(p.hasAttacker());
	return KingAttacksTable[p.get_attacker()];
}

Bitboard KnightAttack(const AttackParams& p)
{
	assert(p.hasAttacker());
	return KnightAttacksTable[p.get_attacker()];
}

Bitboard PawnAttack(const AttackParams& p)
{
	assert(p.hasAttacker() && p.hasColor());
	return PawnAttacksTable[p.get_color()][p.get_attacker()];
}

}


namespace SetAttacks
{

using AttackPtr = Bitboard(*)(const AttackParams&);

void Slide(AttackPtr slow, Magic* attack_table, const int* bits)
{
	AttackParams params;

	for(Square sqr = Square::Start(); sqr < SQUARE_COUNT; ++sqr)
	{
		int perms = 1 << bits[sqr];

		attack_table[sqr].magic = new Bitboard[perms];
		attack_table[sqr].attacks = slow(
			params.set_attacker(sqr).set_blockers(0)
		);

		const Bitboard b = sqr.bitboard();
		if(b & FileType::NotFileA) attack_table[sqr].attacks &= FileType::NotFileA;
		if(b & FileType::NotFileH) attack_table[sqr].attacks &= FileType::NotFileH;
		if(b & RankType::NotRank1) attack_table[sqr].attacks &= RankType::NotRank1;
		if(b & RankType::NotRank8) attack_table[sqr].attacks &= RankType::NotRank8;

		for(int block_sqrs_num = 0; block_sqrs_num < perms; ++block_sqrs_num)
		{
			Bitboard blockers = pdep(block_sqrs_num, attack_table[sqr].attacks);
			int key = pext(blockers, attack_table[sqr].attacks);
			attack_table[sqr].magic[key] = slow(
				params.set_blockers(blockers)
			);
		}
	}
}

void NonSlide(AttackPtr slow, Bitboard* attack_table, AttackParams& p)
{
	for(Square sqr = Square::Start(); sqr < SQUARE_COUNT; ++sqr) {
		p.set_attacker(sqr);
		attack_table[sqr] = slow(p);
	}
}

}


void SetupAttacks()
{
	SetAttacks::Slide(&Slow::BishopAttack, BishopAttacksTable, BishopBits);
	SetAttacks::Slide(&Slow::RookAttack, RookAttacksTable, RookBits);

	AttackParams p;

	SetAttacks::NonSlide(&Slow::KingAttack, KingAttacksTable, p);
	SetAttacks::NonSlide(&Slow::KnightAttack, KnightAttacksTable, p);

	for(Color clr(WHITE); clr.isValid(); clr.next()) {
		p.set_color(clr);
		SetAttacks::NonSlide(&Slow::PawnAttack, PawnAttacksTable[clr], p);
	}
}

Bitboard GetFastAttack(Piece pt, const AttackParams &p)
{
    switch (pt.type())
	{
	case BISHOP: return Fast::BishopAttack(p);
	case ROOK: return Fast::RookAttack(p);
	case QUEEN: return Fast::QueenAttack(p);
	case KING: return Fast::KingAttack(p);
	case KNIGHT: return Fast::KnightAttack(p);
	case PAWN: return Fast::PawnAttack(p);
	default: return Bitboard::Null();
	}
}

Bitboard GetSlowAttack(Piece pt, const AttackParams &p)
{
    switch (pt.type())
	{
	case BISHOP: return Slow::BishopAttack(p);
	case ROOK: return Slow::RookAttack(p);
	case QUEEN: return Slow::QueenAttack(p);
	case KING: return Slow::KingAttack(p);
	case KNIGHT: return Slow::KnightAttack(p);
	case PAWN: return Slow::PawnAttack(p);
	default: return Bitboard::Null();
	}
}

}