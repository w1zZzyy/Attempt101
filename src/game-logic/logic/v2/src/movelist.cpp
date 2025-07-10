#include "movelist.hpp"

#include "attack.hpp"

using namespace game::logic;

void game::logic::DefaultMoveList::piece_moves(
    const Position          &p, 
    const PositionParams    &pp, 
    AttackParams            &ap
) {
    if(pp.is_double_check()) 
        return;

    const Color     us       =   p.get_side();
    const Bitboard  not_occ  =   ~p.get_pieces(us);

    Bitboard pieces = p.get_pieces(
        us,
        BISHOP, ROOK, QUEEN, KNIGHT
    );

    while(pieces) {
        Square from = pieces.poplsb();

        Bitboard moves = AttackManager::Get(
            p.piece_on(from),
            ap.set_attacker(from)
        ) & not_occ;

        if(auto pin_mask = pp.pin_mask(from, p)) {
            moves &= pin_mask;
        }

        if(pp.is_check()) 
            moves &= pp.defensive_squares();

        while(moves) 
            add(from, moves.poplsb(), DEFAULT_MF);
    }
}

void game::logic::DefaultMoveList::king_moves(
    const Position          &p, 
    const PositionParams    &pp, 
    AttackParams            &ap
) {
    const Color     us              =   p.get_side();
    const Square    ksq             =   p.get_pieces(us, KING).lsb();
    const Bitboard  enemy_attacks   =   pp.enemy_attacks();

    auto moves = AttackManager::Get(KING, ap.set_attacker(ksq)) & ~enemy_attacks & ~p.get_pieces(us);
    while(moves)
        add(ksq, moves.poplsb(), DEFAULT_MF);

    if(p.can_castle(KING_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + EAST, S_CASTLE_MF);
    if(p.can_castle(QUEEN_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * WEST, L_CASTLE_MF);
}


