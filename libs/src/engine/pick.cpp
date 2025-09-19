#include "pick.hpp"
#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/square.hpp"
#include <algorithm>
#include <optional>

namespace game::engine 
{

using namespace logic;

namespace 
{

constexpr int PieceValue[PIECE_COUNT] = {
    1500, 1025, 82, 337, 365, 477
};

}

MovePicker::MovePicker(
    const MoveList& __moves, 
    const PositionFixedMemory& __pos, 
    std::optional<logic::Move> __ttMove
) : pos(__pos)
{
    if(__ttMove)
        ttMove = __ttMove.value();

    cur = end = moves;
    for(auto move : __moves) {
        end->setMove(move);
        end->setScore(computeScore(move));
        end++;
    }
}

std::optional<Move> MovePicker::next() {
    if(cur == end) return std::nullopt;

    auto best = std::max_element(cur, end);
    std::swap(*cur, *best); 
    
    Move move = *cur;
    cur++;

    return move;
}

void MovePicker::update(logic::Move bestMovePrevIter) 
{
    std::optional ttMoveOld = ttMove;
    ttMove = bestMovePrevIter;

    cur = moves;
    for(ExtMove* it = cur; it != end; ++it) {
        const Move move = *it;
        if(ttMoveOld && move == ttMoveOld.value()) {
            it->setScore(computeScore(move));
        } else if(move == bestMovePrevIter) {
            it->setScore(INF);
        }
    }
}

int MovePicker::computeScore(Move move) const 
{
    if(move == ttMove)
        return INF;

    if(std::optional targ = captureTarget(move)) 
        return computeCaptureScore(move.from(), move.targ());
    
    return 0;
}

int MovePicker::computeCaptureScore(Square from, Square targ) const 
{
    Piece victim = pos.piece_on(targ);

    int gain[32];
    int depth = 0;

    gain[0] = PieceValue[victim];
    victim = pos.piece_on(from);

    AttackParams params; params.set_attacker(targ);
    Color side = pos.get_side();
    Bitboard occupied = pos.get_occupied(WHITE, BLACK) ^ from.bitboard();
    Bitboard attackers = pos.attacks_to(targ, occupied);

    do 
    {
        ++depth;
        side.swap();

        attackers &= occupied;
        Bitboard side_attackers = attackers & pos.get_occupied(side);

        if(!side_attackers) 
            break;
        
        if(Bitboard pawns = side_attackers & pos.get_pieces(side, PAWN)) {
            updateCapture(occupied, gain + depth, params,  pawns, victim);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
            victim = PAWN;
        }
        else if(Bitboard knights = side_attackers & pos.get_pieces(side, KNIGHT)) {
            updateCapture(occupied, gain + depth, params, knights, victim);
            victim = KNIGHT;
        }
        else if(Bitboard bishops = side_attackers & pos.get_pieces(side, BISHOP)) {
            updateCapture(occupied, gain + depth, params,  bishops, victim);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
            victim = BISHOP;
        }
        else if(Bitboard rooks = side_attackers & pos.get_pieces(side, ROOK)) {
            updateCapture(occupied, gain + depth, params,  rooks, victim);
            attackers |= XrayAttacks(ROOK, params, ROOK, QUEEN);
            victim = ROOK;
        }
        else if(Bitboard queens = side_attackers & pos.get_pieces(side, QUEEN)) {
            updateCapture(occupied, gain + depth, params,  queens, victim);
            attackers |= XrayAttacks(ROOK, params, ROOK, QUEEN);
            attackers |= XrayAttacks(BISHOP, params, BISHOP, QUEEN);
            victim = QUEEN;
        } 
        else { // KING
            updateCapture(occupied, gain + depth, params,  side_attackers, victim);
            victim = KING;
        }

    } while(true);

    while (--depth) {
        int bestOppResp = std::max(-gain[depth - 1], gain[depth]); // либо останавливается, либо продолжает кушать
        gain[depth - 1] = -bestOppResp;
    }

    return gain[0];
}

void MovePicker::updateCapture(Bitboard& occ, int* gain, AttackParams& params, Bitboard least, Piece victim) const {
    assert(gain != nullptr && "gain ptr null");
    occ ^= least.lsb().bitboard();
    params.set_blockers(occ);
    *gain = PieceValue[victim] - *(gain - 1);
} 

std::optional<Square> MovePicker::captureTarget(Move move) const 
{
    const Square targ = move.targ();
    if (move.flag() == EN_PASSANT_MF) return where_passant(move.from(), targ); 
    else if(pos.piece_on(targ).isValid()) { return targ; }
    return std::nullopt;
}


}