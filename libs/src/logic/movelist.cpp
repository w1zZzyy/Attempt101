#include "movelist.hpp"

#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"
#include "move.hpp"
#include "square.hpp"

namespace game::logic
{

namespace
{

constexpr auto prom_list = {Q_PROMOTION_MF, K_PROMOTION_MF, B_PROMOTION_MF, R_PROMOTION_MF};
enum class MoveType {All, Force, Dodge};

void add(Square from, Square targ, MoveFlag flag, Move*& curr) {
    *curr = Move(from, targ, flag); 
    curr++;
}

template<StorageType ST>
void piece_moves(const Position<ST>& pos, Move*& curr, Bitboard target) 
{
    const Color us = pos.get_side();
    AttackParams ap; ap.set_blockers(
        pos.get_occupied(WHITE, BLACK)
    );

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
            add(from, moves.poplsb(), DEFAULT_MF, curr);
    }
}

template<MoveType MT, StorageType T>
void king_moves(const Position<T> &pos, Move*& curr, Bitboard target)
{
    const Color     us              =   pos.get_side();
    const Square    ksq             =   pos.get_pieces(us, KING).lsb();
    const Bitboard  enemy_attacks   =   pos.get_attacks(us.opp());

    Bitboard moves = GetFastAttack(KING, AttackParams{}.set_attacker(ksq)) & ~enemy_attacks & target;
    while(moves)
        add(ksq, moves.poplsb(), DEFAULT_MF, curr);

    if constexpr (MT != MoveType::All) 
        return;

    if(pos.can_castle(KING_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * EAST, S_CASTLE_MF, curr);
    if(pos.can_castle(QUEEN_SIDE_CASTLING, enemy_attacks)) 
        add(ksq, ksq + 2 * WEST, L_CASTLE_MF, curr);
}

void pawn_move_generic(Bitboard moves, std::initializer_list<MoveFlag> flags, int offset_from, Move*& curr) 
{
    while(moves)
    {
        Square targ = moves.poplsb();
        Square from = targ - offset_from;
        for(MoveFlag flag : flags)
            add(from, targ, flag, curr);
    }
}

template <ColorType Us, bool Pinned, StorageType T>
void en_passant_moves(Bitboard pawns, const Position<T>& pos, Move*& curr)
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
                pos.is_blocker(Us, from, targ_pawn) &&
                pos.exposes_discovered_check(from, targ_pawn)
            ) continue;
        }

        add(from, passant, EN_PASSANT_MF, curr);
    }
}

template <MoveType MT, ColorType Us, StorageType T>
void pinned_pawn_moves(Bitboard pawns, const Position<T>& pos, Move*& curr, Bitboard enemy, Bitboard empty)
{
    constexpr ColorType     Them    =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up      =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr Bitboard      TRank3  =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8  =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;
    
    en_passant_moves<Us, true>(pawns, pos, curr);

    AttackParams ap; 
    ap.set_color(Us);

    while(pawns)
    {
        Square from = pawns.poplsb();
        Bitboard pin_mask = pos.pin_mask(from);

        if constexpr(MT == MoveType::All)
        {    
            Bitboard single_up = step<Up>(from.bitboard()) & empty;
            Bitboard double_up = step<Up>(single_up & TRank3) & empty;
            single_up &= pin_mask; 
            double_up &= pin_mask;
            
            if(single_up) 
                add(from, single_up.lsb(), DEFAULT_MF, curr);
            if(double_up)
                add(from, double_up.lsb(), DOUBLE_MF, curr);
        }

        Bitboard captures = GetFastAttack(PAWN, ap.set_attacker(from)) & enemy & pin_mask;

        if(captures & TRank8) {
            Square targ = captures.lsb();
            for(MoveFlag prom : prom_list) 
                add(from, targ, prom, curr);
        }
        else if(captures) 
            add(from, captures.lsb(), DEFAULT_MF, curr); 
    }
}

template<MoveType MT, ColorType Us, StorageType T>
void pawn_moves(const Position<T> &pos, Move*& curr)
{
    constexpr ColorType     Them        =   (Us == WHITE) ? BLACK : WHITE;
    constexpr DirectionType Up          =   (Us == WHITE) ? NORTH : SOUTH;
    constexpr DirectionType Left        =   (Us == WHITE) ? NORTH_WEST : SOUTH_EAST;
    constexpr DirectionType Right       =   (Us == WHITE) ? NORTH_EAST : SOUTH_WEST;
    constexpr Bitboard      TRank3      =   (Us == WHITE) ? RankType::Rank3 : RankType::Rank6;
    constexpr Bitboard      TRank8      =   (Us == WHITE) ? RankType::Rank8 : RankType::Rank1;

    const Bitboard enemy = pos.get_occupied(Them);
    const Bitboard empty = ~pos.get_occupied(WHITE, BLACK);

    
    Bitboard pawns  = pos.get_pieces(Us, PAWN);
    Bitboard pinned = pos.get_pinned(Us) & pawns;
    pawns ^= pinned;

    if constexpr (MT != MoveType::Dodge)
        pinned_pawn_moves<MT, Us>(pinned, pos, curr, enemy, empty);


    Bitboard single_up = step<Up>(pawns) & empty;

    if constexpr (MT != MoveType::Force) {
        Bitboard double_up  = step<Up>(single_up & TRank3) & empty;
        if constexpr (MT == MoveType::Dodge) {
            Bitboard defense = pos.defensive_squares();
            single_up &= defense, double_up &= defense;
        }
        pawn_move_generic(double_up, {DOUBLE_MF}, 2 * Up, curr);
    }

    Bitboard capture_left   = step<Left>(pawns) & enemy;
    Bitboard capture_right  = step<Right>(pawns) & enemy;

    if constexpr (MT == MoveType::Dodge) {
        Bitboard defense = pos.defensive_squares();
        capture_left &= defense, capture_right &= defense;
    }

    Bitboard prom_up    = single_up     & TRank8;   single_up      ^=  prom_up;
    Bitboard prom_left  = capture_left  & TRank8;   capture_left   ^=  prom_left;
    Bitboard prom_right = capture_right & TRank8;   capture_right  ^=  prom_right;

    if constexpr (MT != MoveType::Force)
        pawn_move_generic(single_up, {DEFAULT_MF}, Up, curr);

    pawn_move_generic(capture_left, {DEFAULT_MF}, Left, curr);
    pawn_move_generic(capture_right, {DEFAULT_MF}, Right, curr);
    pawn_move_generic(prom_up, prom_list, Up, curr);
    pawn_move_generic(prom_left, prom_list, Left, curr);
    pawn_move_generic(prom_right, prom_list, Right, curr);


    en_passant_moves<Us, false>(pawns, pos, curr);
}

template<MoveType MT, StorageType T>
void pawn_moves(const Position<T> &pos, Move*& curr, Color us)
{
    us.is(WHITE) 
        ? pawn_moves<MT, WHITE>(pos, curr) 
        : pawn_moves<MT, BLACK>(pos, curr);
}

}

template<MoveGenType MGT, StorageType ST>
void MoveList::generate(const Position<ST> &pos)
{
    const Color us = pos.get_side();
    const Color opp = us.opp();
    constexpr bool IsForced = MGT == MoveGenType::Forced;
    curr = moves;
    Bitboard target = ~pos.get_occupied(us);

    if(!pos.is_double_check()) 
    {
        if(pos.is_check()) {
            piece_moves(pos, curr, target & pos.defensive_squares());
            pawn_moves<MoveType::Dodge>(pos, curr, us);
        }         
        else if constexpr (IsForced) {
            piece_moves(pos, curr, target & pos.get_occupied(opp));
            pawn_moves<MoveType::Force>(pos, curr, us);
        }
        else {
            piece_moves(pos, curr, target);
            pawn_moves<MoveType::All>(pos, curr, us);
        }
    }

    if(pos.is_check()) king_moves<MoveType::Dodge>(pos, curr, target);
    else if constexpr(IsForced) king_moves<MoveType::Force>(pos, curr, target & pos.get_occupied(opp));
    else king_moves<MoveType::All>(pos, curr, target);
}


template void MoveList::generate<MoveGenType::Forced, DynamicStorage>(const PositionDynamicMemory&);
template void MoveList::generate<MoveGenType::NotForced, DynamicStorage>(const PositionDynamicMemory&);
template void MoveList::generate<MoveGenType::Forced, StaticStorage>(const PositionFixedMemory&);
template void MoveList::generate<MoveGenType::NotForced, StaticStorage>(const PositionFixedMemory&);




}