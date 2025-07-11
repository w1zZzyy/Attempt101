#pragma once

#include <cstdint>
#include <optional>

namespace game::logic
{


class Bitboard;
class Square;


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
	static constexpr int Count() noexcept {return 6;}


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
	static constexpr int Count() noexcept {return 2;}
	
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

	K_CASTLING 			= 	(KING_SIDE_CASTLING << 1) 	| 	KING_SIDE_CASTLING,
	k_CASTLING 			= 	(KING_SIDE_CASTLING << 2) 	| 	KING_SIDE_CASTLING,

	Q_CASTLING 			= 	(QUEEN_SIDE_CASTLING << 1) 	| 	QUEEN_SIDE_CASTLING,
	q_CASTLING 			= 	(QUEEN_SIDE_CASTLING << 2) 	| 	QUEEN_SIDE_CASTLING,

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

	static constexpr int Count() noexcept {return 63;}
	static std::optional<CastleType> ByRookSquare(Square rook_sqr);

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
	static const Bitboard FileA;
	static const Bitboard FileB;
	static const Bitboard FileC;
	static const Bitboard FileD;
	static const Bitboard FileE;
	static const Bitboard FileF;
	static const Bitboard FileG;
	static const Bitboard FileH;	
	static const Bitboard NotFileA;
	static const Bitboard NotFileB;
	static const Bitboard NotFileC;
	static const Bitboard NotFileD;
	static const Bitboard NotFileE;
	static const Bitboard NotFileF;
	static const Bitboard NotFileG;
	static const Bitboard NotFileH;
};

class RankType
{
public:
	static const Bitboard Rank1;
	static const Bitboard Rank2;
	static const Bitboard Rank3;
	static const Bitboard Rank4;
	static const Bitboard Rank5;
	static const Bitboard Rank6;
	static const Bitboard Rank7;
	static const Bitboard Rank8;
	static const Bitboard NotRank1;
	static const Bitboard NotRank2;
	static const Bitboard NotRank3;
	static const Bitboard NotRank4;
	static const Bitboard NotRank5;
	static const Bitboard NotRank6;
	static const Bitboard NotRank7;
	static const Bitboard NotRank8;
};


}