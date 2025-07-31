#include "defs.hpp"

#include "bitboard.hpp"
#include "square.hpp"

using namespace game::logic;


std::optional<CastleType> game::logic::Castle::ByRookSquare(Color c, Square rook_sqr)
{
    if(c.is(WHITE)) 
    {
        switch (rook_sqr) {
            case h1: return KING_SIDE_CASTLING;
            case a1: return QUEEN_SIDE_CASTLING;
            default: break;
        }
    }
    else {
        switch (rook_sqr) {
            case h8: return KING_SIDE_CASTLING;
            case a8: return QUEEN_SIDE_CASTLING;
            default: break;
        }
    }
    return std::nullopt;
}

Bitboard game::logic::Castle::king_path() const noexcept
{
    constexpr Bitboard K_PATH = Bitboard::FromSquares(e1, f1, g1);
    constexpr Bitboard k_PATH = Bitboard::FromSquares(e8, f8, g8);
    constexpr Bitboard Q_PATH = Bitboard::FromSquares(e1, d1, c1);
    constexpr Bitboard q_PATH = Bitboard::FromSquares(e8, d8, c8);

    switch (cr)
    {
    case K_CASTLING: return K_PATH;
    case k_CASTLING: return k_PATH;
    case Q_CASTLING: return Q_PATH;
    case q_CASTLING: return q_PATH;
    default:
        break;
    }

    return Bitboard::Null();
}

Bitboard game::logic::Castle::clear_path() const noexcept
{
    constexpr Bitboard K_PATH = Bitboard::FromSquares(f1, g1);
    constexpr Bitboard k_PATH = Bitboard::FromSquares(f8, g8);
    constexpr Bitboard Q_PATH = Bitboard::FromSquares(d1, c1, b1);
    constexpr Bitboard q_PATH = Bitboard::FromSquares(d8, c8, b8);

    switch (cr)
    {
    case K_CASTLING: return K_PATH;
    case k_CASTLING: return k_PATH;
    case Q_CASTLING: return Q_PATH;
    case q_CASTLING: return q_PATH;
    default:
        break;
    }

    return Bitboard::Null();
}

bool game::logic::Castle::has_path() const noexcept
{
    return  cr == K_CASTLING || 
            cr == Q_CASTLING ||
            cr == k_CASTLING ||
            cr == q_CASTLING;
}   

bool game::logic::same_file(Square sq1, Square sq2) noexcept {return sq1 % 8 == sq2 % 8;}
bool game::logic::same_rank(Square sq1, Square sq2) noexcept {return sq1 / 8 == sq2 / 8;}
