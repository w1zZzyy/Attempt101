#include "movelist.hpp"

#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"
#include "move.hpp"
#include "square.hpp"

using namespace game::logic;

constexpr auto prom_list = {Q_PROMOTION_MF, K_PROMOTION_MF, B_PROMOTION_MF, R_PROMOTION_MF};

void game::logic::MoveList::generate(const Position &p)
{
    size = 0;

    PositionParams params;
    params
        .compute_enemy_attackers(p)
        .compute_pins_from_sliders(p);
    
    piece_moves(p, params);
    king_moves(p, params);

    p.get_side().is(WHITE) 
        ? pawn_moves<WHITE>(p, params)
        : pawn_moves<BLACK>(p, params);
}

void game::logic::MoveList::piece_moves(
    const Position &p,
    const PositionParams &pp
) {
    if(pp.is_double_check()) 
        return;

    const Color us = p.get_side();
    AttackParams ap; ap.set_blockers(
        p.get_occupied(WHITE, BLACK)
    );
    
    Bitboard target = ~p.get_occupied(us);
    if(pp.is_check()) 
        target &= pp.defensive_squares();

    Bitboard pieces = p.get_pieces(
        us,
        BISHOP, ROOK, QUEEN, KNIGHT
    );

    while(pieces) 
    {
        Square from = pieces.poplsb();

        Bitboard moves = GetFastAttack(
            p.piece_on(from),
            ap.set_attacker(from)
        ) & target;

        if(Bitboard pin_mask = pp.pin_mask(from, p)) 
            moves &= pin_mask;

        while(moves) 
            add(from, moves.poplsb(), DEFAULT_MF);
    }
}

void game::logic::MoveList::king_moves(
    const Position          &p, 
    const PositionParams    &pp
) {
    const Color     us              =   p.get_side();
    const Square    ksq             =   p.get_pieces(us, KING).lsb();
    const Bitboard  enemy_attacks   =   pp.enemy_attacks();

    AttackParams ap; 
    ap.set_attacker(ksq);

    Bitboard moves = GetFastAttack(KING, ap) & ~enemy_attacks & ~p.get_occupied(us);
    while(moves)
        add(ksq, moves.poplsb(), DEFAULT_MF);

    if(p.can_castle(KING_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * EAST, S_CASTLE_MF);
    if(p.can_castle(QUEEN_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * WEST, L_CASTLE_MF);
}


template<ColorType Us>
inline void game::logic::MoveList::pawn_moves(
    const Position          &p, 
    const PositionParams    &pp
) {
    if(pp.is_double_check()) 
        return;


    constexpr ColorType     Them        =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up          =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr DirectionType Left        =   (Us == WHITE) ? NORTH_WEST : SOUTH_EAST;
    constexpr DirectionType Right       =   (Us == WHITE) ? NORTH_EAST : SOUTH_WEST;
    constexpr Bitboard      TRank3      =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8      =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;

    const Bitboard enemy = p.get_occupied(Them);
    const Bitboard empty = ~p.get_occupied(WHITE, BLACK);

    
    Bitboard pawns  = p.get_pieces(Us, PAWN);
    Bitboard pinned = pp.pinned_pieces() & pawns;
    pawns ^= pinned;

    pinned_pawn_moves<Us>(pinned, p, pp);

    Bitboard single_up      = step<Up>(pawns)                   &   empty;
    Bitboard double_up      = step<Up>(single_up & TRank3)      &   empty;
    Bitboard capture_left   = step<Left>(pawns)                 &   enemy;
    Bitboard capture_right  = step<Right>(pawns)                &   enemy;

    if(pp.is_check()) {
        Bitboard defense = pp.defensive_squares();
        single_up &= defense, double_up &= defense,
        capture_left &= defense, capture_right &= defense;
    }

    Bitboard prom_up    = single_up     & TRank8;   single_up      ^=  prom_up;
    Bitboard prom_left  = capture_left  & TRank8;   capture_left   ^=  prom_left;
    Bitboard prom_right = capture_right & TRank8;   capture_right  ^=  prom_right;


    pawn_move_generic(single_up, {DEFAULT_MF}, Up);
    pawn_move_generic(double_up, {DOUBLE_MF}, 2 * Up);
    pawn_move_generic(capture_left, {DEFAULT_MF}, Left);
    pawn_move_generic(capture_right, {DEFAULT_MF}, Right);

    pawn_move_generic(prom_up, prom_list, Up);
    pawn_move_generic(prom_left, prom_list, Left);
    pawn_move_generic(prom_right, prom_list, Right);


    en_passant_moves<Us, false>(pawns, p, pp);
}

template <ColorType Us>
void game::logic::MoveList::pinned_pawn_moves(
    Bitboard                pawns, 
    const Position&         p,
    const PositionParams&   pp
) {
    if(pp.is_check())
        return;

    constexpr ColorType     Them    =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up      =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr Bitboard      TRank3  =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8  =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;

    const Bitboard enemy_target = p.get_occupied(Them);
    const Bitboard empty_target = ~p.get_occupied(WHITE, BLACK); 

    en_passant_moves<Us, true>(pawns, p, pp);

    AttackParams ap; 
    ap.set_color(Us);

    while(pawns)
    {
        Square      from            =   pawns.poplsb();
        Bitboard    pin_mask        =   pp.pin_mask(from, p);

        Bitboard single_up = step<Up>(from.bitboard()) & empty_target;
        Bitboard double_up = step<Up>(single_up & TRank3) & empty_target;
        Bitboard captures  = GetFastAttack(PAWN, ap.set_attacker(from)) & enemy_target & pin_mask;

        single_up &= pin_mask; 
        double_up &= pin_mask;
        
        if(single_up) 
            add(from, single_up.lsb(), DEFAULT_MF);
        if(double_up)
            add(from, double_up.lsb(), DOUBLE_MF);

        if(captures & TRank8) {
            Square targ = captures.lsb();
            for(MoveFlag prom : prom_list) 
                add(from, targ, prom);
        }
        else if(captures) 
            add(from, captures.lsb(), DEFAULT_MF);
        
    }
}

template <ColorType Us, bool Pinned>
void game::logic::MoveList::en_passant_moves(
    Bitboard                pawns, 
    const Position&         p, 
    const PositionParams&   pp
) {
    if(p.get_passant() == NO_SQUARE)
        return;

    constexpr ColorType     Them    =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up      =   (Us == WHITE) ? NORTH : SOUTH;

    const Square passant    =   p.get_passant();
    const Square targ_pawn  =   passant - Up;

    if(pp.is_check() && !pp.is_attacker(targ_pawn)) 
        return;

    AttackParams passant_params; 
    passant_params.set_color(Them).set_attacker(passant);

    Bitboard passant_attackers = GetFastAttack(PAWN, passant_params) & pawns;
    while(passant_attackers)
    {
        Square from = passant_attackers.poplsb();

        if constexpr (Pinned)
        {
            Bitboard pin_mask = pp.pin_mask(from, p);
            if(!(pin_mask & passant.bitboard())) 
                continue;
        }

        else
        {
            if(
                pp.is_blocker(from, targ_pawn) &&
                pp.exposes_discovered_check(from, targ_pawn, p)
            ) continue;
        }

        add(from, passant, EN_PASSANT_MF);
    }
}


void game::logic::MoveList::pawn_move_generic(
    Bitboard                        moves, 
    std::initializer_list<MoveFlag> flags, 
    int                             offset_from
) {
    while(moves)
    {
        Square targ = moves.poplsb();
        Square from = targ - offset_from;
        for(MoveFlag flag : flags)
            add(from, targ, flag);
    }
}


std::optional<Move> MoveList::find(std::string_view notation) const 
{
    if(4 < notation.size() || notation.size() > 5) 
        return std::nullopt;

    Square from = Square::ToSquare({notation.begin(), 2});
    Square targ = Square::ToSquare({notation.begin() + 2, 2});
    
    std::optional<MoveFlag> flag;
    if (notation.size() == 5) {
        switch (notation.back()) {
            case 'q': flag = Q_PROMOTION_MF; break;
            case 'r': flag = R_PROMOTION_MF; break;
            case 'k': flag = K_PROMOTION_MF; break;
            case 'b': flag = B_PROMOTION_MF; break;
            default: break;
        }
        assert(flag);           
    }

    for(size_t i = 0; i < size; ++i) {
        if(moves[i].from() == from && moves[i].targ() == targ) {
            if(flag && moves[i].flag() == *flag || !flag) {
                return moves[i];
            } 
        }
    }

    return std::nullopt;
}


template void MoveList::pawn_moves<WHITE>(const Position&, const PositionParams&);
template void MoveList::pawn_moves<BLACK>(const Position&, const PositionParams&);

template void MoveList::pinned_pawn_moves<WHITE>(Bitboard, const Position&, const PositionParams&);
template void MoveList::pinned_pawn_moves<BLACK>(Bitboard, const Position&, const PositionParams&);

template void MoveList::en_passant_moves<WHITE, true>(Bitboard, const Position&, const PositionParams&);
template void MoveList::en_passant_moves<WHITE, false>(Bitboard, const Position&, const PositionParams&);
template void MoveList::en_passant_moves<BLACK, true>(Bitboard, const Position&, const PositionParams&);
template void MoveList::en_passant_moves<BLACK, false>(Bitboard, const Position&, const PositionParams&);
