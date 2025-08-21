#include "moveordering.hpp"

#include <algorithm>

namespace game::engine 
{

using namespace logic;

namespace 
{

constexpr int PieceValue[PIECE_COUNT] = {
    1000, // KING
    9, // QUEEN
    1, // PAWN
    3, // KNIGHT
    3, // BISHOP 
    5, // ROOK 
};

}


void MoveOrderer::OrderCaptures(MoveList& moves) const {
    std::sort(moves.data(), moves.data() + moves.get_size(),
        [this](const Move& m1, const Move& m2) {
            return CaptureScore(m1) > CaptureScore(m2);
        }
    );
}

int MoveOrderer::CaptureScore(Move move) const 
{
    Square targ = move.targ();
    Square from = move.from();
    Piece victim = pos->piece_on(targ);

    if(move.flag() == EN_PASSANT_MF) { targ = where_passant(move.from(), targ); }
    else if(!victim.isValid()) { return 0; }

    int gain[32];
    int depth = 0;

    gain[0] = PieceValue[victim];

    AttackParams params; params.set_attacker(targ);
    Color side = pos->get_side();
    Bitboard occupied = pos->get_occupied(WHITE, BLACK) ^ from.bitboard();
    Bitboard attackers = pos->attacks_to(targ, occupied);

    do 
    {
        ++depth;
        side.swap();

        attackers &= occupied;
        Bitboard side_attackers = attackers & pos->get_occupied(side);

        if(!side_attackers) 
            break;
        
        if(Bitboard pawns = side_attackers & pos->get_pieces(side, PAWN)) {
            UpdateCapture(occupied, gain + depth, params,  pawns);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
        }
        else if(Bitboard knights = side_attackers & pos->get_pieces(side, KNIGHT)) {
            UpdateCapture(occupied, gain + depth, params, knights);
        }
        else if(Bitboard bishops = side_attackers & pos->get_pieces(side, BISHOP)) {
            UpdateCapture(occupied, gain + depth, params,  bishops);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
        }
        else if(Bitboard rooks = side_attackers & pos->get_pieces(side, ROOK)) {
            UpdateCapture(occupied, gain + depth, params,  rooks);
            attackers |= XrayAttacks(ROOK, params, ROOK, QUEEN);
        }
        else if(Bitboard queens = side_attackers & pos->get_pieces(side, QUEEN)) {
            UpdateCapture(occupied, gain + depth, params,  queens);
            attackers |= XrayAttacks(ROOK, params, ROOK, QUEEN);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
        } 
        else { // KING
            UpdateCapture(occupied, gain + depth, params,  side_attackers);
        }

    } while(true);

    while (--depth) {
        int bestOppResp = std::max(-gain[depth - 1], gain[depth]); // либо останавливается, либо продолжает кушать
        gain[depth - 1] = -bestOppResp;
    }

    return gain[0];
}

void MoveOrderer::UpdateCapture(Bitboard& occ, int* gain, logic::AttackParams& params, Bitboard least) const {
    assert(gain != nullptr && "gain ptr null");
    Square sqr = least.lsb();
    occ ^= sqr.bitboard();
    params.set_blockers(occ);
    *gain = PieceValue[pos->piece_on(sqr)] - *(gain - 1);
} 

}