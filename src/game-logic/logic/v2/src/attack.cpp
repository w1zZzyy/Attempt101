#include "attack.hpp"

using namespace game::logic;

namespace
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
    Magic() : attacks(0), magic(nullptr) {}
    ~Magic() { delete[] magic; }

    Bitboard attacks;
    Bitboard* magic;
};


static Magic RookAttacksTable[Square::Count()];
static Magic BishopAttacksTable[Square::Count()];
static Bitboard KingAttacksTable[Square::Count()];
static Bitboard KnightAttacksTable[Square::Count()];
static Bitboard PawnAttacksTable[Color::Count()][Square::Count()];



template <DirectionType dir>
Bitboard getDirAttack(Square attacker, Bitboard blockers) noexcept
{
	Bitboard attacks;
    Bitboard piece = attacker.bitboard();
    Bitboard not_blocked = ~blockers;

    while(piece)
    {
        piece = piece.step<dir>();
        attacks |= piece;
        piece &= not_blocked;
    }

    return attacks;
}



}


Bitboard game::logic::BishopAttack::GetSlowAttack(const AttackParams& p) const
{
	AssertParams(p);

	if(p.isDir()) {
		switch (p.get_dir())
		{
		case SOUTH_EAST:   	return getDirAttack<SOUTH_EAST>(p.get_attacker(), p.get_blockers());
		case SOUTH_WEST:   	return getDirAttack<SOUTH_WEST>(p.get_attacker(), p.get_blockers());
		case NORTH_EAST:   	return getDirAttack<NORTH_EAST>(p.get_attacker(), p.get_blockers());
		case NORTH_WEST:   	return getDirAttack<NORTH_WEST>(p.get_attacker(), p.get_blockers());
		default:		 	return 0;
		}
	}

    return
        getDirAttack<SOUTH_EAST>(p.get_attacker(), p.get_blockers()) |
        getDirAttack<SOUTH_WEST>(p.get_attacker(), p.get_blockers()) |
        getDirAttack<NORTH_EAST>(p.get_attacker(), p.get_blockers()) |
        getDirAttack<NORTH_WEST>(p.get_attacker(), p.get_blockers()); 
}

Bitboard game::logic::BishopAttack::GetFastAttack(const AttackParams& p) const
{
	AssertParams(p);

    auto blocked = p.get_blockers() & BishopAttacksTable[p.get_attacker()].attacks;
    int key = pext(blocked, BishopAttacksTable[p.get_attacker()].attacks);

    return BishopAttacksTable[p.get_attacker()].magic[key];
}

Bitboard game::logic::RookAttack::GetSlowAttack(const AttackParams& p) const
{
	AssertParams(p);

	if(p.isDir()) {
		switch (p.get_dir())
		{
		case SOUTH: return getDirAttack<SOUTH>(p.get_attacker(), p.get_blockers());
		case WEST: 	return getDirAttack<WEST>(p.get_attacker(), p.get_blockers());
		case EAST: 	return getDirAttack<EAST>(p.get_attacker(), p.get_blockers());
		case NORTH: return getDirAttack<NORTH>(p.get_attacker(), p.get_blockers());
		default:	return 0;
		}
	}

    return
        getDirAttack<EAST>(p.get_attacker(), p.get_blockers())  |
        getDirAttack<WEST>(p.get_attacker(), p.get_blockers())  |
        getDirAttack<NORTH>(p.get_attacker(), p.get_blockers()) |
        getDirAttack<SOUTH>(p.get_attacker(), p.get_blockers()); 
}

Bitboard game::logic::RookAttack::GetFastAttack(const AttackParams& p) const
{
	AssertParams(p);

    auto blocked = p.get_blockers() & RookAttacksTable[p.get_attacker()].attacks;
    int key = pext(blocked, RookAttacksTable[p.get_attacker()].attacks);

    return RookAttacksTable[p.get_attacker()].magic[key];
}



Bitboard game::logic::QueenAttack::GetSlowAttack(const AttackParams& p) const
{
    return BishopAttack::GetSlowAttack(p) | RookAttack::GetSlowAttack(p);
}

Bitboard game::logic::QueenAttack::GetFastAttack(const AttackParams& p) const
{
    return BishopAttack::GetFastAttack(p) | RookAttack::GetFastAttack(p);
}



Bitboard game::logic::KingAttack::GetSlowAttack(const AttackParams& p) const
{
	AssertParams(p);
    auto king = p.get_attacker().bitboard();

	return 
		king.step<NORTH>() 			|
		king.step<EAST>() 			|
		king.step<WEST>() 			|
		king.step<SOUTH>() 			|
		king.step<NORTH_EAST>() 	|
		king.step<NORTH_WEST>() 	|
		king.step<SOUTH_EAST>() 	|
		king.step<SOUTH_WEST>();
}

Bitboard game::logic::KingAttack::GetFastAttack(const AttackParams& p) const
{
	AssertParams(p);
    return KingAttacksTable[p.get_attacker()];
}



Bitboard game::logic::KnightAttack::GetSlowAttack(const AttackParams& p) const
{
	AssertParams(p);

	Bitboard attacks;
	auto knight = p.get_attacker().bitboard();

	if (!(knight & FileType::FileH))
	{
		attacks |= (knight << 17) | (knight >> 15);
		if (!(knight & FileType::FileG))
			attacks |= (knight << 10) | (knight >> 6);
	}
	if (!(knight & FileType::FileA))
	{
		attacks |= (knight >> 17) | (knight << 15);
		if ((knight & FileType::FileB) == 0)
			attacks |= (knight >> 10) | (knight << 6);
	}

	return attacks;
}

Bitboard game::logic::KnightAttack::GetFastAttack(const AttackParams& p) const
{
	AssertParams(p);
    return KnightAttacksTable[p.get_attacker()];
}



Bitboard game::logic::PawnAttack::GetSlowAttack(const AttackParams& p) const
{
	AssertParams(p);

    auto clr 	= 	p.get_color();
	auto pawn 	= 	p.get_attacker().bitboard();

	if(clr.is(WHITE)) {
		return 
			pawn.step<NORTH_WEST>() |
            pawn.step<NORTH_EAST>();
	} 
	if(clr.is(BLACK)) {
		return 
			pawn.step<SOUTH_WEST>() |
            pawn.step<SOUTH_EAST>();
	}

	return 0;
}

Bitboard game::logic::PawnAttack::GetFastAttack(const AttackParams &p) const
{
	AssertParams(p);
    return PawnAttacksTable[p.get_color()][p.get_attacker()];
}


void game::logic::AttackManager::Setup()
{
	attacks[KING] 	= std::make_unique<KingAttack>();
	attacks[QUEEN] 	= std::make_unique<QueenAttack>();
	attacks[PAWN] 	= std::make_unique<PawnAttack>();
	attacks[BISHOP] = std::make_unique<BishopAttack>();
	attacks[KNIGHT] = std::make_unique<KnightAttack>();
	attacks[ROOK] 	= std::make_unique<RookAttack>();


	auto SetSlideAttacks = [](const AttackPtr& attack_ptr, Magic* attack_table, const int* bits)
	{
		Bitboard edges = 
			RankType::Rank1 | RankType::Rank8 |
			FileType::FileA | FileType::FileH;

		AttackParams params;

		for(Square sqr = Square::Start(); sqr < Square::Count(); ++sqr)
		{
			int perms = 1ULL << bits[sqr];

			attack_table[sqr].magic = new Bitboard[perms];
			attack_table[sqr].attacks = attack_ptr->GetSlowAttack(
				params.set_attacker(sqr).set_blockers(0)
			) & ~edges;

			for(int block_sqrs_num = 0; block_sqrs_num < perms; ++block_sqrs_num)
			{
				Bitboard blockers = pdep(block_sqrs_num, attack_table[sqr].attacks);
				int key = pext(blockers, attack_table[sqr].attacks);
				attack_table[sqr].magic[key] = attack_ptr->GetSlowAttack(
					params.set_attacker(sqr).set_blockers(blockers)
				);
			}
		}
	};
	auto SetNonSlideAttacks = [](const AttackPtr& attack_ptr, Bitboard* attack_table, AttackParams& p)
	{
		for(Square sqr = Square::Start(); sqr < Square::Count(); ++sqr) {
			p.set_attacker(sqr);
			attack_table[sqr] = attack_ptr->GetSlowAttack(p);
		}
	};


	SetSlideAttacks(attacks[BISHOP], BishopAttacksTable, BishopBits);
	SetSlideAttacks(attacks[ROOK], RookAttacksTable, RookBits);


	AttackParams p;

	SetNonSlideAttacks(attacks[KING], KingAttacksTable, p);
	SetNonSlideAttacks(attacks[KNIGHT], KnightAttacksTable, p);

	for(Color clr(WHITE); clr.isValid(); clr.next()) {
		p.set_color(clr);
		SetNonSlideAttacks(attacks[PAWN], PawnAttacksTable[clr], p);
	}
}

Bitboard game::logic::AttackManager::Get(Piece attacker, const AttackParams& p)
{
    return attacks[attacker]->GetFastAttack(p);
}

void game::logic::ISlideAttack::AssertParams(const AttackParams& p) const
{
	assert(p.isAttacker() && p.isBlockers());
}

void game::logic::INonSlideAttack::AssertParams(const AttackParams& p) const
{
	assert(p.isAttacker());
}

void game::logic::PawnAttack::AssertParams(const AttackParams& p) const
{
	assert(p.isAttacker() && p.isColor());
}
