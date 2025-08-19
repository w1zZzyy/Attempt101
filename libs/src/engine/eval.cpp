#include "eval.hpp"

namespace game::engine
{

// using Pestos values

constexpr int mg_value[logic::PIECE_COUNT] = {0, 1025, 82, 337, 365, 477};
constexpr int eg_value[logic::PIECE_COUNT] = {0, 936, 94, 281, 297, 512};

constexpr int gamephaseInc[logic::PIECE_COUNT] = {0, 4, 0, 1, 1, 2};

constexpr int mg_piece_table[logic::PIECE_COUNT][logic::SQUARE_COUNT] = 
{
    // king
    {
        -65,  23,  16, -15, -56, -34,   2,  13,
        29,  -1, -20,  -7,  -8,  -4, -38, -29,
        -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14
    },

    // queen
    {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
        -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
        -1, -18,  -9,  10, -15, -25, -31, -50
    },

    // pawn
    {
        0,   0,   0,   0,   0,   0,  0,   0,
        98, 134,  61,  95,  68, 126, 34, -11,
        -6,   7,  26,  31,  65,  56, 25, -20,
        -14,  13,   6,  21,  23,  12, 17, -23,
        -27,  -2,  -5,  12,  17,   6, 10, -25,
        -26,  -4,  -4, -10,   3,   3, 33, -12,
        -35,  -1, -20, -23, -15,  24, 38, -22,
        0,   0,   0,   0,   0,   0,  0,   0
    },

    // knight
    {
        -167, -89, -34, -49,  61, -97, -15, -107,
        -73, -41,  72,  36,  23,  62,   7,  -17,
        -47,  60,  37,  65,  84, 129,  73,   44,
        -9,  17,  19,  53,  37,  69,  18,   22,
        -13,   4,  16,  13,  28,  19,  21,   -8,
        -23,  -9,  12,  10,  19,  17,  25,  -16,
        -29, -53, -12,  -3,  -1,  18, -14,  -19,
        -105, -21, -58, -33, -17, -28, -19,  -23
    },

    // bishop
    {
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -16,  37,  43,  40,  35,  50,  37,  -2,
        -4,   5,  19,  50,  37,  37,   7,  -2,
        -6,  13,  13,  26,  34,  12,  10,   4,
        0,  15,  15,  15,  14,  27,  18,  10,
        4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21
    },

    // rook
    {
        32,  42,  32,  51, 63,  9,  31,  43,
        27,  32,  58,  62, 80, 67,  26,  44,
        -5,  19,  26,  36, 17, 45,  61,  16,
        -24, -11,   7,  26, 24, 35,  -8, -20,
        -36, -26, -12,  -1,  9, -7,   6, -23,
        -45, -25, -16, -17,  3,  0,  -5, -33,
        -44, -16, -20,  -9, -1, 11,  -6, -71,
        -19, -13,   1,  17, 16,  7, -37, -26
    }
};

constexpr int eg_piece_table[logic::PIECE_COUNT][logic::SQUARE_COUNT] = 
{
    // king
    {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
        10,  17,  23,  15,  20,  45,  44,  13,
        -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  27,  23,   9, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
    },

    // queen
    {
        -9,  22,  22,  27,  27,  19,  10,  20,
        -17,  20,  32,  41,  58,  25,  30,   0,
        -20,   6,   9,  49,  47,  35,  19,   9,
        3,  22,  24,  45,  57,  40,  57,  36,
        -18,  28,  19,  47,  31,  34,  39,  23,
        -16, -27,  15,   6,   9,  17,  10,   5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43,  -5, -32, -20, -41
    },

    // pawn
    {
        0,   0,   0,   0,   0,   0,   0,   0,
        178, 173, 158, 134, 147, 132, 165, 187,
        94, 100,  85,  67,  56,  53,  82,  84,
        32,  24,  13,   5,  -2,   4,  17,  17,
        13,   9,  -3,  -7,  -7,  -8,   3,  -1,
        4,   7,  -6,   1,   0,  -5,  -1,  -8,
        13,   8,   8,  10,  13,   0,   2,  -7,
        0,   0,   0,   0,   0,   0,   0,   0
    },

    // knight
    {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64
    },

    // bishop
    {
        -14, -21, -11,  -8, -7,  -9, -17, -24,
        -8,  -4,   7, -12, -3, -13,  -4, -14,
        2,  -8,   0,  -1, -2,   6,   0,   4,
        -3,   9,  12,   9, 14,  10,   3,   2,
        -6,   3,  13,  19,  7,  10,  -3,  -9,
        -12,  -3,   8,  10, 13,   3,  -7, -15,
        -14, -18,  -7,  -1,  4,  -9, -15, -27,
        -23,  -9, -23,  -5, -9, -16,  -5, -17
    },

    // rook
    {
        13, 10, 18, 15, 12,  12,   8,   5,
        11, 13, 13, 11, -3,   3,   8,   3,
        7,  7,  7,  5,  4,  -3,  -5,  -3,
        4,  3, 13,  1,  2,   1,  -1,   2,
        3,  5,  8,  4, -5,  -6,  -8, -11,
        -4,  0, -5, -1, -7, -12,  -8, -16,
        -6, -6,  0,  2, -9,  -9, -11,  -3,
        -9,  2,  3, -1, -5, -13,   4, -20
    }
};

using namespace logic;

int mg_table[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];
int eg_table[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];


void Eval::Setup()
{
    for(Piece p = KING; p <= ROOK; p.next()) {
        for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
            mg_table[WHITE][p][sqr] = mg_value[p] + mg_piece_table[p][sqr];
            eg_table[WHITE][p][sqr] = eg_value[p] + eg_piece_table[p][sqr];
            mg_table[BLACK][p][sqr] = mg_value[p] + mg_piece_table[p][sqr.flip()];
            eg_table[BLACK][p][sqr] = eg_value[p] + eg_piece_table[p][sqr.flip()];
        }
    }
}

void Eval::init(const PositionFixedMemory& pos) 
{
    game_phase = 0;
    mg[WHITE] = 0;
    mg[BLACK] = 0;
    eg[WHITE] = 0;
    eg[BLACK] = 0;

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        Piece p = pos.piece_on(sqr);
        Color c = pos.piece_clr_on(sqr);
        if(p.isValid()) add_piece(c, p, sqr);
    }
}


void Eval::update(const PositionFixedMemory& pos, logic::Move move)
{
    const Square from = move.from();
    const Square targ = move.targ();
    const MoveFlag flag = move.flag();
    const Color side_curr = pos.get_side();
    const Color side_moved = side_curr.opp();
    
    if(const Piece captured = pos.get_captured(); captured.isValid()) remove_piece(side_curr, captured, targ);
    else if(flag == EN_PASSANT_MF) remove_piece(side_curr, PAWN, where_passant(from, targ));

    switch (flag)
    {
    case S_CASTLE_MF:
        castle(side_moved, from, targ, targ + EAST, from + EAST);
        break;
    case L_CASTLE_MF:
        castle(side_moved, from, targ, targ + 2 * WEST, from + WEST);
        break;
    case Q_PROMOTION_MF:
    case R_PROMOTION_MF:
    case K_PROMOTION_MF:
    case B_PROMOTION_MF:
    {
        const Piece p = pos.piece_on(targ);
        mg[side_moved] += mg_table[side_moved][p][targ] - mg_table[side_moved][PAWN][from];
        eg[side_moved] += eg_table[side_moved][p][targ] - eg_table[side_moved][PAWN][from];
        game_phase += gamephaseInc[p] - gamephaseInc[PAWN];
        break;
    }
    default:
    {
        update_piece(side_moved, pos.piece_on(targ), from, targ);
        break;
    }
    
    }
}

void Eval::rollback(const PositionFixedMemory &pos, logic::Move move)
{
    const Color side_curr = pos.get_side();
    const Color side_moved = side_curr.opp();
    const Square from = move.from();
    const Square targ = move.targ();
    const MoveFlag flag = move.flag();

    if(const Piece captured = pos.get_captured(); captured.isValid()) add_piece(side_curr, captured, targ);
    else if(flag == EN_PASSANT_MF) add_piece(side_curr, PAWN, where_passant(from, targ));

    switch (flag)
    {
    case S_CASTLE_MF:
        castle(side_moved, targ, from, from + EAST, targ + EAST);
        break;
    case L_CASTLE_MF:
        castle(side_moved, targ, from, from + WEST, targ + 2 * WEST);
        break;
    case Q_PROMOTION_MF:
    case R_PROMOTION_MF:
    case K_PROMOTION_MF:
    case B_PROMOTION_MF:
    {
        const Piece p = pos.piece_on(targ);
        mg[side_moved] -= mg_table[side_moved][p][targ] + mg_table[side_moved][PAWN][from];
        eg[side_moved] -= eg_table[side_moved][p][targ] + eg_table[side_moved][PAWN][from];
        game_phase -= gamephaseInc[p] + gamephaseInc[PAWN];
        break;
    }
    default:
        update_piece(side_moved, pos.piece_on(targ), targ, from);
        break;
    }
}

int Eval::score(const PositionFixedMemory& pos) const
{
    const Color side = pos.get_side();

    int mg_score = mg[side] - mg[side.opp()];
    int eg_score = eg[side] - eg[side.opp()];
    
    int mg_phase = (game_phase > 24) ? 24 : game_phase;
    int eg_phase = 24 - mg_phase;

    return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}

void Eval::update_piece(logic::Color side, logic::Piece piece, logic::Square from, logic::Square targ)
{
    mg[side] += mg_table[side][piece][targ] - mg_table[side][piece][from];
    eg[side] += eg_table[side][piece][targ] - eg_table[side][piece][from];
}

void Eval::add_piece(logic::Color side, logic::Piece piece, logic::Square sqr)
{
    mg[side] += mg_table[side][piece][sqr];
    eg[side] += eg_table[side][piece][sqr];
    game_phase += gamephaseInc[piece];
}

void Eval::remove_piece(logic::Color side, logic::Piece piece, logic::Square from)
{
    mg[side] -= mg_table[side][piece][from];
    eg[side] -= eg_table[side][piece][from];
    game_phase -= gamephaseInc[piece];
}

void Eval::castle(logic::Color side, logic::Square kf, logic::Square kt, logic::Square rf, logic::Square rt)
{
    update_piece(side, KING, kf, kt);
    update_piece(side, ROOK, rf, rt);
}

}