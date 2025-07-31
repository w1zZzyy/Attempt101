#include "movelist.hpp"

#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"
#include "move.hpp"
#include "square.hpp"

using namespace game::logic;

constexpr auto prom_list = {Q_PROMOTION_MF, K_PROMOTION_MF, B_PROMOTION_MF, R_PROMOTION_MF};

void game::logic::MoveList::generate(Position &pos)
{
    size = 0;

    pos
        .compute_enemy_attackers()
        .compute_pins_from_sliders();
    
    piece_moves(pos);
    king_moves(pos);

    pos.get_side().is(WHITE) 
        ? pawn_moves<WHITE>(pos)
        : pawn_moves<BLACK>(pos);
}

void game::logic::MoveList::piece_moves(const Position& pos)
{
    if(pos.is_double_check()) 
        return;

    const Color us = pos.get_side();
    AttackParams ap; ap.set_blockers(
        pos.get_occupied(WHITE, BLACK)
    );
    
    Bitboard target = ~pos.get_occupied(us);
    if(pos.is_check()) 
        target &= pos.defensive_squares();

    Bitboard pieces = pos.get_pieces(
        us,
        BISHOP, ROOK, QUEEN, KNIGHT
    );

    while(pieces) 
    {
        Square from = pieces.poplsb();

        Bitboard moves = GetFastAttack(
            pos.piece_on(from),
            ap.set_attacker(from)
        ) & target;

        if(Bitboard pin_mask = pos.pin_mask(from)) 
            moves &= pin_mask;

        while(moves) 
            add(from, moves.poplsb(), DEFAULT_MF);
    }
}

void game::logic::MoveList::king_moves(const Position &pos)
{
    const Color     us              =   pos.get_side();
    const Square    ksq             =   pos.get_pieces(us, KING).lsb();
    const Bitboard  enemy_attacks   =   pos.enemy_attacks();

    AttackParams ap; 
    ap.set_attacker(ksq);

    Bitboard moves = GetFastAttack(KING, ap) & ~enemy_attacks & ~pos.get_occupied(us);
    while(moves)
        add(ksq, moves.poplsb(), DEFAULT_MF);

    if(pos.can_castle(KING_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * EAST, S_CASTLE_MF);
    if(pos.can_castle(QUEEN_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * WEST, L_CASTLE_MF);
}


template<ColorType Us>
inline void game::logic::MoveList::pawn_moves(const Position &pos)
{
    if(pos.is_double_check()) 
        return;


    constexpr ColorType     Them        =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up          =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr DirectionType Left        =   (Us == WHITE) ? NORTH_WEST : SOUTH_EAST;
    constexpr DirectionType Right       =   (Us == WHITE) ? NORTH_EAST : SOUTH_WEST;
    constexpr Bitboard      TRank3      =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8      =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;

    const Bitboard enemy = pos.get_occupied(Them);
    const Bitboard empty = ~pos.get_occupied(WHITE, BLACK);

    
    Bitboard pawns  = pos.get_pieces(Us, PAWN);
    Bitboard pinned = pos.pinned_pieces() & pawns;
    pawns ^= pinned;

    pinned_pawn_moves<Us>(pinned, pos);

    Bitboard single_up      = step<Up>(pawns)                   &   empty;
    Bitboard double_up      = step<Up>(single_up & TRank3)      &   empty;
    Bitboard capture_left   = step<Left>(pawns)                 &   enemy;
    Bitboard capture_right  = step<Right>(pawns)                &   enemy;

    if(pos.is_check()) {
        Bitboard defense = pos.defensive_squares();
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


    en_passant_moves<Us, false>(pawns, pos);
}

template <ColorType Us>
void game::logic::MoveList::pinned_pawn_moves(Bitboard pawns, const Position& pos)
{
    if(pos.is_check())
        return;

    constexpr ColorType     Them    =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up      =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr Bitboard      TRank3  =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8  =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;

    const Bitboard enemy_target = pos.get_occupied(Them);
    const Bitboard empty_target = ~pos.get_occupied(WHITE, BLACK); 

    en_passant_moves<Us, true>(pawns, pos);

    AttackParams ap; 
    ap.set_color(Us);

    while(pawns)
    {
        Square      from            =   pawns.poplsb();
        Bitboard    pin_mask        =   pos.pin_mask(from);

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
void game::logic::MoveList::en_passant_moves(Bitboard pawns, const Position& pos)
{
    if(pos.get_passant() == NO_SQUARE)
        return;

    constexpr ColorType     Them    =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up      =   (Us == WHITE) ? NORTH : SOUTH;

    const Square passant    =   pos.get_passant();
    const Square targ_pawn  =   passant - Up;

    if(pos.is_check() && !pos.is_attacker(targ_pawn)) 
        return;

    AttackParams passant_params; 
    passant_params.set_color(Them).set_attacker(passant);

    Bitboard passant_attackers = GetFastAttack(PAWN, passant_params) & pawns;
    while(passant_attackers)
    {
        Square from = passant_attackers.poplsb();

        if constexpr (Pinned)
        {
            Bitboard pin_mask = pos.pin_mask(from);
            if(!(pin_mask & passant.bitboard())) 
                continue;
        }

        else
        {
            if(
                pos.is_blocker(from, targ_pawn) &&
                pos.exposes_discovered_check(from, targ_pawn)
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
