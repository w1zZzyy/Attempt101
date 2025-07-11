#include "defs.hpp"

#include "bitboard.hpp"
#include "square.hpp"

using namespace game::logic;


const Bitboard FileType::FileA = Bitboard(0x0101010101010101ULL);
const Bitboard FileType::FileB = FileType::FileA << 1;
const Bitboard FileType::FileC = FileType::FileA << 2;
const Bitboard FileType::FileD = FileType::FileA << 3;
const Bitboard FileType::FileE = FileType::FileA << 4;
const Bitboard FileType::FileF = FileType::FileA << 5;
const Bitboard FileType::FileG = FileType::FileA << 6;
const Bitboard FileType::FileH = FileType::FileA << 7;
const Bitboard FileType::NotFileA = ~FileType::FileA;
const Bitboard FileType::NotFileB = ~FileType::FileB;
const Bitboard FileType::NotFileC = ~FileType::FileC;
const Bitboard FileType::NotFileD = ~FileType::FileD;
const Bitboard FileType::NotFileE = ~FileType::FileE;
const Bitboard FileType::NotFileF = ~FileType::FileF;
const Bitboard FileType::NotFileG = ~FileType::FileG;
const Bitboard FileType::NotFileH = ~FileType::FileH;

const Bitboard RankType::Rank1 = Bitboard(0xFFULL);
const Bitboard RankType::Rank2 = RankType::Rank1 << 8;
const Bitboard RankType::Rank3 = RankType::Rank1 << 16;
const Bitboard RankType::Rank4 = RankType::Rank1 << 24;
const Bitboard RankType::Rank5 = RankType::Rank1 << 32;
const Bitboard RankType::Rank6 = RankType::Rank1 << 40;
const Bitboard RankType::Rank7 = RankType::Rank1 << 48;
const Bitboard RankType::Rank8 = RankType::Rank1 << 56;
const Bitboard RankType::NotRank1 = ~RankType::Rank1;
const Bitboard RankType::NotRank2 = ~RankType::Rank2;
const Bitboard RankType::NotRank3 = ~RankType::Rank3;
const Bitboard RankType::NotRank4 = ~RankType::Rank4;
const Bitboard RankType::NotRank5 = ~RankType::Rank5;
const Bitboard RankType::NotRank6 = ~RankType::Rank6;
const Bitboard RankType::NotRank7 = ~RankType::Rank7;
const Bitboard RankType::NotRank8 = ~RankType::Rank8;


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
    static const Bitboard K_PATH = Bitboard::FromSquares(e1, f1, g1);
    static const Bitboard k_PATH = Bitboard::FromSquares(e8, f8, g8);
    static const Bitboard Q_PATH = Bitboard::FromSquares(e1, d1, c1);
    static const Bitboard q_PATH = Bitboard::FromSquares(e8, d8, c8);

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
    static const Bitboard K_PATH = Bitboard::FromSquares(f1, g1);
    static const Bitboard k_PATH = Bitboard::FromSquares(f8, g8);
    static const Bitboard Q_PATH = Bitboard::FromSquares(d1, c1, b1);
    static const Bitboard q_PATH = Bitboard::FromSquares(d8, c8, b8);

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