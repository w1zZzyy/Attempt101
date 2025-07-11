#include "position.hpp"

#include "attack.hpp"

#include <cctype>
#include <sstream>


using namespace game::logic;

game::logic::Position::Position() : 
    Position(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        std::make_unique<DynamicStateStorage>()
    )
{
}

game::logic::Position::Position(std::string_view fen, StateStoragePtr &&_st) : st(std::move(_st))
{
    auto& new_st = st->create();

    std::stringstream ss(fen.data());

    std::string pos, castle, passant;
	char clr;

    ss >> pos >> clr >> castle >> passant >> new_st.rule50; 

    {
        Square sqr(a8);
        for(auto symb : pos)
        {
            if (symb == '/') {
                sqr = sqr.down();
                continue;
            }

            if(std::isdigit(symb)) {
                int empty = symb - '0';
                for (int i = 0; i < empty; ++i)
                    sqr = sqr.right();
                continue;
            }

            switch (symb)
            {
            case 'K': add_piece(WHITE, KING, sqr);    break;
            case 'Q': add_piece(WHITE, QUEEN, sqr);   break;
            case 'P': add_piece(WHITE, PAWN, sqr);    break;
            case 'N': add_piece(WHITE, KNIGHT, sqr);  break;
            case 'R': add_piece(WHITE, ROOK, sqr);    break;
            case 'B': add_piece(WHITE, BISHOP, sqr);  break;
            case 'k': add_piece(BLACK, KING, sqr);    break;
            case 'q': add_piece(BLACK, QUEEN, sqr);   break;
            case 'p': add_piece(BLACK, PAWN, sqr);    break;
            case 'n': add_piece(BLACK, KNIGHT, sqr);  break;
            case 'r': add_piece(BLACK, ROOK, sqr);    break;
            case 'b': add_piece(BLACK, BISHOP, sqr);  break;
            default: break;
            }

            sqr = sqr.right();
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
}


void game::logic::Position::do_move(Move move)
{
    auto& old_st = st->top();
    if(old_st.passant.isValid())
        old_st.hash.updateEnPassant(old_st.passant);

    auto& new_st = st->create();
    new_st.move = move;

    Square from = move.from();
    Square targ = move.targ();
    MoveFlag flag = move.flag();

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
        new_st.hash.updateEnPassant(targ - (targ > from) ? NORTH : SOUTH);
        break;
    case EN_PASSANT_MF:
        remove_piece(side.opp(), targ - (targ > from) ? NORTH : SOUTH);
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
    default:
        break;
    }

    if(types[targ].isValid()) {
        try_to_update_castle(side.opp(), targ); // capture opp rook
        new_st.captured = types[targ];
        remove_piece(side.opp(), targ);
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

    auto& old_st = st->rollback();
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
        add_piece<HashUpdate>(side.opp(), PAWN, targ - (targ > from) ? NORTH : SOUTH);
        break;
    case Q_PROMOTION_MF:
        replace<HashUpdate>(QUEEN, targ);
        break;
    case R_PROMOTION_MF:
        replace<HashUpdate>(ROOK, targ);
        break;
    case K_PROMOTION_MF:
        replace<HashUpdate>(KNIGHT, targ);
        break;
    case B_PROMOTION_MF:
        replace<HashUpdate>(BISHOP, targ);
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


void game::logic::Position::update_castle(Color c, CastleType ct)
{
    auto& curr_st = st->top();
    const CastleRightsType cr = curr_st.castle.extract(c, ct);

    if(cr == NO_CASTLING) {
        return;
    }

    curr_st.hash.updateCastle(curr_st.castle);
    curr_st.castle.rem(cr);
    curr_st.hash.updateCastle(curr_st.castle);
}

void game::logic::Position::try_to_update_castle(Color c, Square maybe_rook)
{
    if(types[maybe_rook].is(ROOK)) {
        if(auto ct = Castle::ByRookSquare(maybe_rook)) {
            update_castle(c, *ct);
        }
    }
}

PositionParams& game::logic::PositionParams::compute_enemy_attackers(const Position& position)
{
    const Color opp = position.get_side().opp();

    AttackParams attack_params;
    attack_params
        .set_color(opp)
        .set_blockers(position.get_occupied(WHITE, BLACK));

    Bitboard    pieces = position.get_occupied(opp);
    Bitboard    king   = position.get_pieces(position.get_side(), KING);
    Square      ksq    = king.lsb();

    while(pieces)
    {
        Square from = pieces.poplsb();
        attack_params.set_attacker(from);
        
        Bitboard piece_attacks = AttackManager::Get(position.piece_on(from), attack_params);
        this->attackers |= piece_attacks;

        if(piece_attacks & king) {
            this->checkers  |=  from.bitboard();
            this->defense   &=  position.piece_on(from).is(KNIGHT) ? from.bitboard() : between(ksq, from);
        }
    }

    return *this;
}

PositionParams& game::logic::PositionParams::compute_pins_from_sliders(const Position& position)
{
    const Color c = position.get_side();
    const Square ksq = position.get_pieces(c, KING).lsb();

    AttackParams attack_params;
    attack_params
        .set_blockers(0)
        .set_attacker(ksq);

    Bitboard snipers = 
        AttackManager::Get(ROOK, attack_params) & position.get_pieces(c.opp(), ROOK, QUEEN) |
        AttackManager::Get(BISHOP, attack_params) & position.get_pieces(c.opp(), BISHOP, QUEEN);
    Bitboard occ = position.get_occupied(WHITE, BLACK) ^ snipers;

    while(snipers)
    {
        Square sniper_sq = snipers.poplsb();
        Bitboard b = between(ksq, sniper_sq) & occ;

        if(b) {
            this->king_blockers |= b;
            b &= position.get_occupied(c);
            if(b.count() == 1) {
                this->pinned |= b;
            }
        }
    }

    return *this;
}

bool game::logic::PositionParams::exposes_discovered_check(Square from, Square targ, const Position &p) const
{
    const Color us = p.get_side();

    AttackParams discovered_check_params;
    discovered_check_params
        .set_attacker(p.get_pieces(us, KING).lsb())
        .set_blockers(p.get_occupied(WHITE, BLACK) ^ Bitboard::FromSquares(from, targ));

    Bitboard sliders_on_line = p.get_pieces(us.opp(), ROOK, QUEEN) & same_line(from, targ);
    if(sliders_on_line & AttackManager::Get(ROOK, discovered_check_params))
        return false;

    return true;
}

Bitboard game::logic::PositionParams::pin_mask(Square sqr, const Position &p) const
{
    if(!(pinned & sqr.bitboard())) {
        return Bitboard::Null();
    }

    const Color c = p.get_side();
    const Square ksq = p.get_pieces(c, KING).lsb();

    return same_line(ksq, sqr);
}
