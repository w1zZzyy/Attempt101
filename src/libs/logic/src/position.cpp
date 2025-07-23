#include "position.hpp"

#include "attack.hpp"
#include "bitboard.hpp"
#include "defs.hpp"
#include "square.hpp"
#include "storage.hpp"

#include <cctype>
#include <sstream>
#include <string>


using namespace game::logic;

void game::logic::Position::Init()
{
    SetupAttacks();
    Square::Setup();
    Zobrist::Setup();
}


game::logic::Position::Position(std::string_view fen, StateStoragePtr &&_st) : 
    st(std::move(_st)) {
    set_fen(fen);
}

Position& Position::set_fen(std::string_view fen)
{
    auto& new_st = st->create();

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

void game::logic::Position::do_move(Move move)
{
    State& old_st = st->top();
    if(old_st.passant.isValid())
        old_st.hash.updateEnPassant(old_st.passant);

    State& new_st   =   st->create();
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
        update_passant(targ - (targ > from ? NORTH : SOUTH));
        break;
    case EN_PASSANT_MF:
        remove_piece(side.opp(), targ - (targ > from ? NORTH : SOUTH));
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

void game::logic::Position::undo_move()
{
    constexpr bool HashUpdate = false;

    Move last_move = st->top().move;
    Piece captured = st->top().captured;
    
    Square from = last_move.from();
    Square targ = last_move.targ();
    MoveFlag flag = last_move.flag();

    State& old_st = st->rollback();
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
        add_piece<HashUpdate>(side.opp(), PAWN, targ - ((targ > from) ? NORTH : SOUTH));
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


bool game::logic::Position::can_castle(CastleType ct, Bitboard enemy_attacks) const
{
    const Castle cr = st->top().castle.extract(side, ct);
    return 
          cr.has_path()                     &&
        !(cr.king_path()  & enemy_attacks)  &&
        !(cr.clear_path() & get_occupied(WHITE, BLACK));
}

void game::logic::Position::update_passant(Square sqr)
{
    State& curr_st = st->top();
    curr_st.hash.updateEnPassant(sqr);
    curr_st.passant = sqr;
}

void game::logic::Position::update_castle(Color c, CastleType ct)
{
    State& curr_st = st->top();
    const CastleRightsType cr = curr_st.castle.extract(c, ct);

    if(cr == NO_CASTLING) 
        return;

    curr_st.hash.updateCastle(curr_st.castle);
    curr_st.castle.rem(cr);
    curr_st.hash.updateCastle(curr_st.castle);
}

void game::logic::Position::try_to_update_castle(Color c, Square maybe_rook)
{
    if(types[maybe_rook].is(ROOK)) {
        if(auto ct = Castle::ByRookSquare(c, maybe_rook)) {
            update_castle(c, *ct);
        }
    }
}


std::string game::logic::Position::fen() const noexcept
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

    State& curr_st = st->top();
    
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


bool Position::is_draw() const
{
    return not_enough_pieces() || st->is_draw();
}

bool Position::not_enough_pieces() const noexcept
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


PositionParams& game::logic::PositionParams::compute_enemy_attackers(const Position& position)
{
    const Color us  = position.get_side();
    const Color opp = us.opp();

    Bitboard    pawns   = position.get_pieces(opp, PAWN);
    Bitboard    pieces  = position.get_occupied(opp) ^ pawns;
    Bitboard    king    = position.get_pieces(us, KING);
    Square      ksq     = king.lsb();

    AttackParams attack_params;
    attack_params.set_blockers(position.get_occupied(WHITE, BLACK) ^ king);

    while(pieces)
    {
        Square  from = pieces.poplsb();
        Piece   type = position.piece_on(from);
        attack_params.set_attacker(from);
        
        Bitboard piece_attacks = GetFastAttack(type, attack_params);
        this->attackers |= piece_attacks;

        if(piece_attacks & king) {
            this->checkers  |=  from.bitboard();
            this->defense   &=  position.piece_on(from).is(KNIGHT) 
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
            .set_color(us)
            .set_attacker(ksq);
            
        Bitboard pawn_checkers = GetFastAttack(PAWN, attack_params) & pawns;

        this->checkers  |=  pawn_checkers;
        this->defense   &=  pawn_checkers;
    }

    return *this;
}

PositionParams& game::logic::PositionParams::compute_pins_from_sliders(const Position& position)
{
    const Color c = position.get_side();
    const Square ksq = position.get_pieces(c, KING).lsb();

    AttackParams attack_params;
    attack_params
        .set_blockers(Bitboard::Null())
        .set_attacker(ksq);

    Bitboard snipers = 
        GetFastAttack(ROOK, attack_params) & position.get_pieces(c.opp(), ROOK, QUEEN) |
        GetFastAttack(BISHOP, attack_params) & position.get_pieces(c.opp(), BISHOP, QUEEN);
    Bitboard occ = position.get_occupied(WHITE, BLACK) ^ snipers;

    while(snipers)
    {
        Square sniper = snipers.poplsb();
        Bitboard b = between(ksq, sniper) & occ;

        if(b) {
            this->king_blockers |= b;
            if(b.count() == 1 && b & position.get_occupied(c)) {
                this->pinned |= b;
            }
        }
    }

    return *this;
}

bool game::logic::PositionParams::exposes_discovered_check(Square from, Square targ, const Position &p) const
{
    if(!same_rank(from, targ)) {
        return false;
    }

    const Color us = p.get_side();

    AttackParams discovered_check_params;
    discovered_check_params
        .set_attacker(p.get_pieces(us, KING).lsb())
        .set_blockers(p.get_occupied(WHITE, BLACK) ^ Bitboard::FromSquares(from, targ));

    Bitboard sliders_on_line = p.get_pieces(us.opp(), ROOK, QUEEN) & line_bb(from, targ);
    if(sliders_on_line & GetFastAttack(ROOK, discovered_check_params))
        return true;

    return false;
}

Bitboard game::logic::PositionParams::pin_mask(Square sqr, const Position &p) const
{
    if(!(pinned & sqr.bitboard())) {
        return Bitboard::Null();
    }

    const Color c = p.get_side();
    const Square ksq = p.get_pieces(c, KING).lsb();

    return line_bb(ksq, sqr);
}


std::ostream& operator<<(std::ostream& out, const game::logic::Position& position)
{
    constexpr char PieceName[Color::Count()][Piece::Count()] =
	{
		{'K', 'Q', 'P', 'N', 'B', 'R'},
		{'k', 'q', 'p', 'n', 'b', 'r'}
	};

    for (int y = 7; y >= 0; --y)
	{
		for (int x = 0; x < 7; ++x)
			out << "-----";

		out << "\n";
		out << y + 1 << " | ";

		for (int x = 0; x < 8; ++x)
		{
			Square sqr(y * 8 + x);
			Piece piece = position.piece_on(sqr);

			if (piece != NO_PIECE)
			{
				Bitboard piece_bb = sqr.bitboard();

                out << ((piece_bb & position.get_occupied(WHITE)) 
                    ? PieceName[WHITE][piece] 
                    : PieceName[BLACK][piece]);
			}
			else 
                out << ' ';

			out << " | ";
		}
		out << "\n";
	}

    return out;
}