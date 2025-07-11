#include "bitboard.hpp"


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

template <DirectionType dir>
inline Bitboard Bitboard::step() noexcept
{
	U64 _bb = bb;

	if constexpr (dir == DirectionType::NORTH) _bb <<= 8;
	if constexpr (dir == DirectionType::SOUTH) _bb >>= 8;
	if constexpr (dir == DirectionType::EAST)  (_bb &= FileType::NotFileH) <<= 1;
	if constexpr (dir == DirectionType::WEST)  (_bb &= FileType::NotFileA) >>= 1;

	if constexpr (dir == DirectionType::NORTH_WEST) (_bb &= FileType::NotFileA) <<= 7;
	if constexpr (dir == DirectionType::NORTH_EAST) (_bb &= FileType::NotFileH) <<= 9;
	if constexpr (dir == DirectionType::SOUTH_EAST) (_bb &= FileType::NotFileH) >>= 7;
	if constexpr (dir == DirectionType::SOUTH_WEST) (_bb &= FileType::NotFileA) >>= 9;

	return Bitboard(_bb);
}
template<> Bitboard Bitboard::step<DirectionType::EAST>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::WEST>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::NORTH>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::SOUTH>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::NORTH_EAST>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::NORTH_WEST>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::SOUTH_EAST>() noexcept;
template<> Bitboard Bitboard::step<DirectionType::SOUTH_WEST>() noexcept;