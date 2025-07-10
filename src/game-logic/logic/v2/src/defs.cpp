#include "defs.hpp"

#include "bitboard.hpp"

using namespace game::logic;

std::optional<CastleType> game::logic::Castle::ByRookSquare(Square rook_sqr)
{
    switch (rook_sqr)
    {
    case h1: return KING_SIDE_CASTLING;
    case h8: return KING_SIDE_CASTLING;
    case a1: return QUEEN_SIDE_CASTLING;
    case a8: return QUEEN_SIDE_CASTLING;	
    default: break;
    }
    return std::nullopt;
}

Bitboard game::logic::Castle::king_path() const noexcept
{
    static const Bitboard K_PATH = Bitboard(e1, f1, g1);
    static const Bitboard k_PATH = Bitboard(e8, f8, g8);
    static const Bitboard Q_PATH = Bitboard(e1, d1, c1);
    static const Bitboard q_PATH = Bitboard(e8, d8, c8);

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
    static const Bitboard K_PATH = Bitboard(f1, g1);
    static const Bitboard k_PATH = Bitboard(f8, g8);
    static const Bitboard Q_PATH = Bitboard(d1, c1, b1);
    static const Bitboard q_PATH = Bitboard(d8, c8, b8);

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