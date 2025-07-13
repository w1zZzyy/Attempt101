#include "bitboard.hpp"

#include <iostream>

#ifdef _WIN64 // MSVC
	#include <intrin.h>
	#include <nmmintrin.h>
#elif defined (__GNUC__) // GCC or CLANG
	#include <x86intrin.h>
#endif


using namespace game::logic;


Square Bitboard::lsb() const
{
#if defined(_WIN64) && defined(_MSC_VER)
	return Square(_tzcnt_u64(pos)); 
#elif defined (__GNUC__) 
	return Square(__builtin_ctzll(bb));
#endif
}

Square Bitboard::poplsb()
{
    Square index = lsb();
	bb &= (bb - 1);
	return index;
}

int game::logic::Bitboard::count() const
{
#if defined(_WIN64) && defined(_MSC_VER)
	return _mm_popcnt_u64(position); 
#elif defined (__GNUC__) 
	return __builtin_popcountll(bb);
#endif	   
}

int game::logic::pext(
	const Bitboard &blockers, 
	const Bitboard &attacks
) {
    return _pext_u64(blockers.bb, attacks.bb); 
}

Bitboard game::logic::pdep(
	int num, 
	const Bitboard &attacks
) {
    return Bitboard(_pdep_u64(num, attacks.bb)); 
}

void game::logic::Bitboard::print() const
{
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			Bitboard mask = 1ULL << (i * 8 + j);
			if (mask.bb & bb)
				std::cout << "1";
			else
				std::cout << "0";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}