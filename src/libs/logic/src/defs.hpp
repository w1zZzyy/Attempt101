#pragma once

#include <cstdint>
#include <optional>

#include "bitboard.hpp"
#include "square.hpp"

namespace game::logic
{


constexpr int PIECE_COUNT = 6;
constexpr int COLOR_COUNT = 2;
constexpr int CASTLE_COUNT = 63;
constexpr int SQUARE_COUNT = 64;
constexpr int MAX_MOVES_COUNT = 218;
constexpr int MAX_HISTORY_SIZE = 20;


using U64 = uint64_t;
using U8  = uint8_t;


enum DirectionType : int8_t
{
	NORTH	=	 8,
	SOUTH	=	 -8,	
	EAST	=	 1,
	WEST	=	 -1,

	NORTH_WEST = NORTH + WEST,
	NORTH_EAST = NORTH + EAST,
	SOUTH_WEST = SOUTH + WEST,
	SOUTH_EAST = SOUTH + EAST,

	NO_DIRECTION = -128
};


enum PieceType : U8
{
	KING,
	QUEEN,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	NO_PIECE
};

class Piece
{
public:
	
	Piece() noexcept : piece(NO_PIECE) {}
	Piece(PieceType _p) noexcept : piece(_p) {}
	Piece(const Piece& _p) noexcept : piece(_p.piece) {}

	void set(PieceType _p) noexcept {piece = _p;}
	void next() {piece = static_cast<PieceType>(piece + 1);}
	bool is(PieceType p) const noexcept {return piece == p;}
	bool isValid() const noexcept {return piece != NO_PIECE;}

	PieceType type() const noexcept {return piece;}

	operator int() const noexcept {return piece;}

private:

	PieceType piece;

};


enum ColorType : U8
{
	WHITE,
	BLACK,
	ANY_COLOR
};

class Color
{
public:
	
	Color() noexcept : color(ANY_COLOR) {}
	Color(const Color& c) noexcept : color(c.color) {}
	Color(ColorType _c) noexcept : color(_c) {}
	Color(int _c) : color(static_cast<ColorType>(_c)) {}

	void next() noexcept {
		if(isValid()) 
			color = ColorType(1 + int(color));
	}
	void set(char c) noexcept {
		color = (c == 'w') ? WHITE : (c == 'b') ? BLACK 
		: ANY_COLOR;
	}
	void swap() noexcept {
		color = ColorType(opp().color);
	}

	Color opp() const noexcept {
		return ColorType(1 - int(color));
	}

	bool is(ColorType c) const noexcept {return color == c;}
	bool isValid() const noexcept {return is(WHITE) || is(BLACK);}

	operator int() const noexcept {return int(color);}

private:

	ColorType color;

};


enum CastleType : U8 
{
	KING_SIDE_CASTLING 	= 	1,
	QUEEN_SIDE_CASTLING	= 	1 << 3,
	BOTH_SIDES_CASTLING = 	KING_SIDE_CASTLING | QUEEN_SIDE_CASTLING
};

enum CastleRightsType : U8
{
	NO_CASTLING 		= 	0,

	K_CASTLING 			= 	KING_SIDE_CASTLING << 1,
	k_CASTLING 			= 	KING_SIDE_CASTLING << 2,

	Q_CASTLING 			= 	QUEEN_SIDE_CASTLING << 1,
	q_CASTLING 			= 	QUEEN_SIDE_CASTLING << 2,

	KQ_CASTLING			=	 K_CASTLING 	| 	Q_CASTLING,
	KQk_CASTLING		=	 KQ_CASTLING 	| 	k_CASTLING,
	KQq_CASTLING		=	 KQ_CASTLING 	| 	q_CASTLING,
	KQkq_CASTLING		=	 KQk_CASTLING 	| 	q_CASTLING,
	Kk_CASTLING			=	 K_CASTLING 	| 	k_CASTLING,
	Kq_CASTLING			=	 K_CASTLING 	| 	q_CASTLING,
	Kkq_CASTLING		=	 Kk_CASTLING 	| 	q_CASTLING,
	Qk_CASTLING			=	 Q_CASTLING 	| 	k_CASTLING,
	Qq_CASTLING			=	 Q_CASTLING 	| 	q_CASTLING,
	Qkq_CASTLING		=	 Qk_CASTLING 	| 	q_CASTLING,
	kq_CASTLING 		= 	 k_CASTLING 	| 	q_CASTLING
};

class Castle
{
public:

	static std::optional<CastleType> ByRookSquare(Color c, Square rook_sqr);

	Castle() = default;
	Castle(const Castle& c) : cr(c.cr) {}
	Castle(CastleRightsType c) noexcept : cr(c) {}

	void operator = (const Castle& c) noexcept {cr = c.cr;}

	Castle& add(CastleRightsType _cr) noexcept {
		cr = static_cast<CastleRightsType>(cr | _cr);
		return *this;
	}
	Castle& rem(CastleRightsType _cr) noexcept {
		cr = static_cast<CastleRightsType>(cr & ~_cr);
		return *this;
	}

	CastleRightsType extract(Color c, CastleType ct) const noexcept {	
		return static_cast<CastleRightsType>(cr & (ct << (c + 1))); 
	}

	bool available(CastleRightsType _cr) const noexcept {
		return (cr & _cr) == _cr;
	}

	Bitboard king_path() const noexcept;
	Bitboard clear_path() const noexcept;

	bool has_path() const noexcept;
	
	operator int() const noexcept {return cr;}

private:

	CastleRightsType cr{NO_CASTLING};

};


class FileType
{
public:
	inline static constexpr Bitboard FileA = Bitboard(0x0101010101010101ULL);
	inline static constexpr Bitboard FileB = FileA << 1;
	inline static constexpr Bitboard FileC = FileA << 2;
	inline static constexpr Bitboard FileD = FileA << 3;
	inline static constexpr Bitboard FileE = FileA << 4;
	inline static constexpr Bitboard FileF = FileA << 5;
	inline static constexpr Bitboard FileG = FileA << 6;
	inline static constexpr Bitboard FileH = FileA << 7;
	inline static constexpr Bitboard NotFileA = ~FileA;
	inline static constexpr Bitboard NotFileB = ~FileB;
	inline static constexpr Bitboard NotFileC = ~FileC;
	inline static constexpr Bitboard NotFileD = ~FileD;
	inline static constexpr Bitboard NotFileE = ~FileE;
	inline static constexpr Bitboard NotFileF = ~FileF;
	inline static constexpr Bitboard NotFileG = ~FileG;
	inline static constexpr Bitboard NotFileH = ~FileH;
};

class RankType
{
public:
	inline static constexpr Bitboard Rank1 = Bitboard(0xFFULL);
	inline static constexpr Bitboard Rank2 = Rank1 << 8;
	inline static constexpr Bitboard Rank3 = Rank1 << 16;
	inline static constexpr Bitboard Rank4 = Rank1 << 24;
	inline static constexpr Bitboard Rank5 = Rank1 << 32;
	inline static constexpr Bitboard Rank6 = Rank1 << 40;
	inline static constexpr Bitboard Rank7 = Rank1 << 48;
	inline static constexpr Bitboard Rank8 = Rank1 << 56;
	inline static constexpr Bitboard NotRank1 = ~Rank1;
	inline static constexpr Bitboard NotRank2 = ~Rank2;
	inline static constexpr Bitboard NotRank3 = ~Rank3;
	inline static constexpr Bitboard NotRank4 = ~Rank4;
	inline static constexpr Bitboard NotRank5 = ~Rank5;
	inline static constexpr Bitboard NotRank6 = ~Rank6;
	inline static constexpr Bitboard NotRank7 = ~Rank7;
	inline static constexpr Bitboard NotRank8 = ~Rank8;
};


template<DirectionType dir>
Bitboard step(Bitboard b) noexcept
{
	if constexpr (dir == NORTH) b <<= 8;
	if constexpr (dir == SOUTH) b >>= 8;
	if constexpr (dir == EAST)  (b &= FileType::NotFileH) <<= 1;
	if constexpr (dir == WEST)  (b &= FileType::NotFileA) >>= 1;

	if constexpr (dir == NORTH_WEST) (b &= FileType::NotFileA) <<= 7;
	if constexpr (dir == NORTH_EAST) (b &= FileType::NotFileH) <<= 9;
	if constexpr (dir == SOUTH_EAST) (b &= FileType::NotFileH) >>= 7;
	if constexpr (dir == SOUTH_WEST) (b &= FileType::NotFileA) >>= 9;

	return b;
}


}