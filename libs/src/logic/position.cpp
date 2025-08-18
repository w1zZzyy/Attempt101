#include "position.hpp"

#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"
#include "square.hpp"
#include "storage.hpp"

#include <cctype>
#include <sstream>
#include <string>


namespace game::logic
{

template<StorageType Policy>
void Position<Policy>::Init()
{
    static bool init = false;
    if(!init)
    {
        SetupAttacks();
        Square::Setup();
        Zobrist::Setup();
        init = true;
    }
}

template<StorageType Policy>
Position<Policy>::Position(std::string_view fen)
{
    set_fen(fen);
}

template<StorageType Policy>
Position<Policy>& Position<Policy>::set_fen(std::string_view fen)
{
    auto& new_st = st.create();

    std::stringstream ss(fen.data());

    std::string pos, castle, passant;
	char clr;

    ss >> pos >> clr >> castle >> passant >> new_st.rule50; 

    {   
        int row = 7, col = 0;
        for(char symb : pos)
        {
            if (symb == '/') {
                --row;
                col = 0;
                continue;
            }

            if(std::isdigit(symb))  {
                col += symb - '0';
                continue;
            }

            Square sqr(row * 8 + col);

            switch (symb)
            {
            case 'K': add_piece(WHITE, KING,    sqr);   break;
            case 'Q': add_piece(WHITE, QUEEN,   sqr);   break;
            case 'P': add_piece(WHITE, PAWN,    sqr);   break;
            case 'N': add_piece(WHITE, KNIGHT,  sqr);   break;
            case 'R': add_piece(WHITE, ROOK,    sqr);   break;
            case 'B': add_piece(WHITE, BISHOP,  sqr);   break;
            case 'k': add_piece(BLACK, KING,    sqr);   break;
            case 'q': add_piece(BLACK, QUEEN,   sqr);   break;
            case 'p': add_piece(BLACK, PAWN,    sqr);   break;
            case 'n': add_piece(BLACK, KNIGHT,  sqr);   break;
            case 'r': add_piece(BLACK, ROOK,    sqr);   break;
            case 'b': add_piece(BLACK, BISHOP,  sqr);   break;
            default: break;
            }

            col++;
        }
    }

    side.set(clr);
    new_st.hash.updateSide();

    for (auto& c : castle)
	{
		switch (c)
		{
		case 'K': new_st.castle.add(K_CASTLING); break;
		case 'Q': new_st.castle.add(Q_CASTLING); break;
		case 'k': new_st.castle.add(k_CASTLING); break;
		case 'q': new_st.castle.add(q_CASTLING); break;
		default:
			break;
		}
	}
    new_st.hash.updateCastle(new_st.castle);

    if(passant == "-" || passant.empty()) {
        new_st.passant = NO_SQUARE;
    }
    else {
        new_st.passant = Square::ToSquare(passant); 
        new_st.hash.updateEnPassant(new_st.passant);
    }

    return *this;
}

template<StorageType Policy>
void Position<Policy>::do_move(Move move)
{
    State& old_st = st.top();
    if(old_st.passant.isValid())
        old_st.hash.updateEnPassant(old_st.passant);

    State& new_st   =   st.create();
    new_st.move     =   move;

    Square from     =   move.from();
    Square targ     =   move.targ();
    MoveFlag flag   =   move.flag();

    switch (flag)
    {
    case DEFAULT_MF:
        if(types[from].is(KING)) update_castle(side, BOTH_SIDES_CASTLING);
        else try_to_update_castle(side, from);
        break;
    case S_CASTLE_MF:
        move_piece(targ + EAST, from + EAST);
        update_castle(side, BOTH_SIDES_CASTLING);
        break;
    case L_CASTLE_MF:
        move_piece(targ + 2 * WEST, from + WEST);
        update_castle(side, BOTH_SIDES_CASTLING);
        break;
    case DOUBLE_MF:
        update_passant(where_passant(from, targ));
        break;
    case EN_PASSANT_MF:
        remove_piece(side.opp(), where_passant(from, targ));
        break;
    case Q_PROMOTION_MF:
        replace(QUEEN, from);
        break;
    case R_PROMOTION_MF:
        replace(ROOK, from);
        break;
    case K_PROMOTION_MF:
        replace(KNIGHT, from);
        break;
    case B_PROMOTION_MF:
        replace(BISHOP, from);
        break;
    }

    if(types[targ].isValid()) {
        try_to_update_castle(side.opp(), targ); // capture opp rook
        new_st.captured = types[targ];
        remove_piece(side.opp(), targ);
        new_st.rule50 = 0;
    } else if(!types[from].is(PAWN)) {
        new_st.rule50++;
    } else new_st.rule50 = 0;

    move_piece(from, targ);

    side.swap();
    new_st.hash.updateSide();
}

template<StorageType Policy>
void Position<Policy>::undo_move()
{
    constexpr bool HashUpdate = false;

    const Move last_move = st.top().move;
    const Piece captured = st.top().captured;
    
    const Square from = last_move.from();
    const Square targ = last_move.targ();
    const MoveFlag flag = last_move.flag();

    st.rollback();
    side.swap();

    switch (flag)
    {
    case S_CASTLE_MF:
        move_piece<HashUpdate>(from + EAST, targ + EAST);
        break;
    case L_CASTLE_MF:
        move_piece<HashUpdate>(from + WEST, targ + 2 * WEST);
        break;
    case EN_PASSANT_MF:
        add_piece<HashUpdate>(side.opp(), PAWN, where_passant(from, targ));
        break;
    case Q_PROMOTION_MF:
    case R_PROMOTION_MF:
    case K_PROMOTION_MF:
    case B_PROMOTION_MF:
        replace<HashUpdate>(PAWN, targ);
        break;
    default:
        break;
    }

    move_piece<HashUpdate>(targ, from);

    if(captured.isValid()) {
        add_piece<HashUpdate>(side.opp(), captured, targ);
    }
}

template<StorageType Policy>
bool Position<Policy>::can_castle(CastleType ct, Bitboard enemy_attacks) const
{
    const Castle cr = st.top().castle.extract(side, ct);
    return 
          cr.has_path()                     &&
        !(cr.king_path()  & enemy_attacks)  &&
        !(cr.clear_path() & get_occupied(WHITE, BLACK));
}

template<StorageType Policy>
void Position<Policy>::update_passant(Square sqr)
{
    State& curr_st = st.top();
    curr_st.hash.updateEnPassant(sqr);
    curr_st.passant = sqr;
}

template<StorageType Policy>
void Position<Policy>::update_castle(Color c, CastleType ct)
{
    State& curr_st = st.top();
    const CastleRightsType cr = curr_st.castle.extract(c, ct);

    if(cr == NO_CASTLING) 
        return;

    curr_st.hash.updateCastle(curr_st.castle);
    curr_st.castle.rem(cr);
    curr_st.hash.updateCastle(curr_st.castle);
}

template<StorageType Policy>
void Position<Policy>::try_to_update_castle(Color c, Square maybe_rook)
{
    if(types[maybe_rook].is(ROOK)) {
        if(auto ct = Castle::ByRookSquare(c, maybe_rook)) {
            update_castle(c, *ct);
        }
    }
}

template<StorageType Policy>
std::string Position<Policy>::fen() const noexcept
{
    std::ostringstream res;

    for(int rank = 7; rank >= 0; --rank)
    {
        for(int file = 0; file < 8; ++file)
        {
            Square sqr = Square::ToSquare(rank * 8 + file);

            int empty = 0;
            while(piece_on(sqr) == NO_PIECE && file < 8)
            {
                file++;
                sqr = Square::ToSquare(rank * 8 + file);
                ++empty;
            }

            if(empty) res << empty;
            if(file == 8) continue;

            Bitboard sqr_bb = sqr.bitboard();

            switch (piece_on(sqr))
            {
            case KING:
                res << (sqr_bb & occupied[WHITE] ? 'K' : 'k');
                break;
            case QUEEN:
                res << (sqr_bb & occupied[WHITE] ? 'Q' : 'q');
                break;
            case PAWN:
                res << (sqr_bb & occupied[WHITE] ? 'P' : 'p');
                break;
            case ROOK:
                res << (sqr_bb & occupied[WHITE] ? 'R' : 'r');
                break;
            case KNIGHT:
                res << (sqr_bb & occupied[WHITE] ? 'N' : 'n');
                break;
            case BISHOP:
                res << (sqr_bb & occupied[WHITE] ? 'B' : 'b');
                break;
            }
        }

        if(rank != 0) 
            res << '/';
    }

    res << (side.is(WHITE) ? " w " : " b ");

    const State& curr_st = st.top();
    
    if(curr_st.castle != NO_CASTLING) {
        if(curr_st.castle.available(K_CASTLING)) res << 'K';
        if(curr_st.castle.available(k_CASTLING)) res << 'Q';
        if(curr_st.castle.available(Q_CASTLING)) res << 'Q';
        if(curr_st.castle.available(q_CASTLING)) res << 'q';
    } else {
        res << '-';
    }
    res << ' ';
    res << (curr_st.passant.isValid() ? curr_st.passant.to_string() : "-") << " ";
    res << curr_st.rule50;

    return res.str();
}

template<StorageType Policy>
bool Position<Policy>::is_draw() const
{
    return not_enough_pieces() || st.repetition() || st.top().rule50 == 50;
}

template<StorageType Policy>
bool Position<Policy>::not_enough_pieces() const noexcept
{
    int total_pieces = get_occupied(WHITE, BLACK).count();
    return (
        total_pieces == 2 ||  // 2 kings
        total_pieces == 3 && ( // 1 king vs king + bishop / knight
            get_pieces(WHITE, BISHOP, KNIGHT).count() == 1 ||
            get_pieces(BLACK, BISHOP, KNIGHT).count() == 1
        )
    );
}

template<StorageType Policy>
Position<Policy>& Position<Policy>::compute_enemy_attackers()
{
    this->attackers = Bitboard::Null();
    this->checkers = Bitboard::Null();
    this->defense = Bitboard::Full();

    const Color opp = side.opp();

    Bitboard    pawns   = get_pieces(opp, PAWN);
    Bitboard    pieces  = get_occupied(opp) ^ pawns;
    Bitboard    king    = get_pieces(side, KING);
    Square      ksq     = king.lsb();

    AttackParams attack_params;
    attack_params.set_blockers(get_occupied(WHITE, BLACK) ^ king);

    while(pieces)
    {
        Square  from = pieces.poplsb();
        Piece   type = piece_on(from);
        attack_params.set_attacker(from);
        
        Bitboard piece_attacks = GetFastAttack(type, attack_params);
        this->attackers |= piece_attacks;

        if(piece_attacks & king) {
            this->checkers  |=  from.bitboard();
            this->defense   &=  piece_on(from).is(KNIGHT) 
                                ? from.bitboard() 
                                : between(ksq, from);
        }
    }

    Bitboard pawn_attacks = (opp.is(WHITE)) 
                            ? step<NORTH_EAST>(pawns) | step<NORTH_WEST>(pawns)
                            : step<SOUTH_EAST>(pawns) | step<SOUTH_WEST>(pawns);

    this->attackers |= pawn_attacks;
    
    if (pawn_attacks & king) 
    {
        attack_params
            .set_color(side)
            .set_attacker(ksq);
            
        Bitboard pawn_checkers = GetFastAttack(PAWN, attack_params) & pawns;

        this->checkers  |=  pawn_checkers;
        this->defense   &=  pawn_checkers;
    }

    return *this;
}

template<StorageType Policy>
Position<Policy>& Position<Policy>::compute_pins_from_sliders()
{
    this->king_blockers = Bitboard::Null();
    this->pinned = Bitboard::Null();

    const Color opp = side.opp();
    const Square ksq = get_pieces(side, KING).lsb();

    AttackParams attack_params;
    attack_params
        .set_blockers(Bitboard::Null())
        .set_attacker(ksq);

    Bitboard snipers = 
        GetFastAttack(ROOK, attack_params) & get_pieces(opp, ROOK, QUEEN) |
        GetFastAttack(BISHOP, attack_params) & get_pieces(opp, BISHOP, QUEEN);
    Bitboard occ = get_occupied(WHITE, BLACK) ^ snipers;

    while(snipers)
    {
        Square sniper = snipers.poplsb();
        Bitboard b = between(ksq, sniper) & occ;

        if(b) {
            this->king_blockers |= b;
            if(b.count() == 1 && b & get_occupied(side)) {
                this->pinned |= b;
            }
        }
    }

    return *this;
}

template<StorageType Policy>
bool Position<Policy>::exposes_discovered_check(Square from, Square targ) const
{
    if(!same_rank(from, targ)) {
        return false;
    }

    AttackParams discovered_check_params;
    discovered_check_params
        .set_attacker(get_pieces(side, KING).lsb())
        .set_blockers(get_occupied(WHITE, BLACK) ^ Bitboard::FromSquares(from, targ));

    Bitboard sliders_on_line = get_pieces(side.opp(), ROOK, QUEEN) & line_bb(from, targ);
    if(sliders_on_line & GetFastAttack(ROOK, discovered_check_params))
        return true;

    return false;
}

template<StorageType Policy>
Bitboard Position<Policy>::pin_mask(Square sqr) const
{
    if(!(pinned & sqr.bitboard())) {
        return Bitboard::Null();
    }

    const Square ksq = get_pieces(side, KING).lsb();

    return line_bb(ksq, sqr);
}


template class Position<DynamicStorage>;
template class Position<StaticStorage>;

}