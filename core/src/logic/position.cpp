#include "position.hpp"
#include "attack.hpp"
#include "bitboard.hpp"

#include <sstream>

namespace Core::Logic
{

void PositionBase::Setup() 
{
    static bool init = false;
    if(!init) {
        SetupAttacks();
        Square::Setup();
        Zobrist::Setup();
        init = true;
    }
}

Color PositionBase::GetPieceColor(Square s) const { 
    const Bitboard b = s.bitboard();
    return (
        (b & occupied[WHITE]) ? WHITE :
        (b & occupied[BLACK]) ? BLACK :
        NO_COLOR
    );
}

void PositionBase::AddPiece(Color color, Piece piece, Square sqr) noexcept
{
    Bitboard new_piece = sqr.bitboard();
    pieces[color][piece] |= new_piece;
    occupied[color] |= new_piece;
    types[sqr] = piece;
}

void PositionBase::AddPiece(Color color, Piece piece, Square sqr, Zobrist &hash) noexcept
{
    AddPiece(color, piece, sqr);
    hash.updateSquare(color, piece, sqr);
}

Piece PositionBase::RemovePiece(Color color, Square sqr) noexcept 
{
    Bitboard del_b = sqr.bitboard();
    Piece piece = types[sqr];

    assert(piece.isValid());
    
    pieces[color][piece] ^= del_b;
    occupied[color] ^= del_b;
    types[sqr] = NO_PIECE;

    return piece;
}

void PositionBase::RemovePiece(Color color, Square sqr, Zobrist &hash) noexcept 
{
    Piece piece = RemovePiece(color, sqr);
    hash.updateSquare(color, piece, sqr);
}

Piece PositionBase::MovePiece(Square from, Square targ) noexcept 
{ 
    Piece piece = types[from];
    Bitboard move_bb = from.bitboard() | targ.bitboard();

    assert(piece.isValid());

    pieces[side][piece] ^= move_bb;
	occupied[side] ^= move_bb;
    types[from] = NO_PIECE;
    types[targ] = piece;

    return piece;
}

void PositionBase::MovePiece(Square from, Square targ, Zobrist &hash) noexcept 
{
    Piece piece = MovePiece(from, targ);
    hash    
        .updateSquare(side, piece, from)
        .updateSquare(side, piece, targ);
}

Piece PositionBase::ReplacePiece(Piece new_piece, Square sqr) noexcept 
{ 
    Bitboard piece_bb = sqr.bitboard();
    Piece old_piece = types[sqr];

    assert(old_piece.isValid());

    pieces[side][old_piece] ^= piece_bb;
    types[sqr] = new_piece;
    pieces[side][new_piece] ^= piece_bb;

    return old_piece;
}

void PositionBase::ReplacePiece(Piece new_piece, Square sqr, Zobrist &hash) noexcept 
{
    Piece old_piece = ReplacePiece(new_piece, sqr);
    hash 
        .updateSquare(side, old_piece, sqr)
        .updateSquare(side, new_piece, sqr);
}

bool PositionAttacks::CanPassant(Square from, Square targ) const noexcept 
{
    const Square ksq = GetPieces(side, KING).lsb();
    const int kr = ksq.rank();

    if(kr != from.rank()) // from.rank = targ.rank (bcs passant)
        return true;

    const int d = (from > ksq ? 1 : -1);
    const Color opp = side.opp();

    for(Square sqr = ksq + d; sqr.rank() == kr; sqr += d) {
        if(sqr == from || sqr == targ) continue;
        if(
            Piece piece = GetPiece(sqr); 
            piece.isValid()
        ) {
            bool isOpp = GetPieceColor(sqr) == opp;
            bool isSlide = piece.is(QUEEN) || piece.is(ROOK);
            return (!isOpp || !isSlide);
        }
    }

    return true;
}

Bitboard PositionAttacks::GetPinMask(Square sqr) const noexcept {
  if (!(pinned & sqr.bitboard()))
    return Bitboard::Null();

  const Square ksq = GetPieces(side, KING).lsb();

  return line_bb(ksq, sqr);
}

Bitboard PositionAttacks::GetAttacksTo(Square sqr, Bitboard occ) const noexcept 
{
    AttackParams params;
    params
        .set_attacker(sqr) 
        .set_blockers(occ);

    Bitboard attacks;

    for(Color clr = WHITE; clr.isValid(); clr.next()) 
    {
        params.set_color(clr);
        attacks |= (GetFastAttack(PAWN, params) & GetPieces(clr.opp(), PAWN));
    }

    attacks 
        |= (GetFastAttack(ROOK, params) & GetPieces(ANY_COLOR, ROOK, QUEEN)) 
        |= (GetFastAttack(BISHOP, params) & GetPieces(ANY_COLOR, BISHOP, QUEEN)) 
        |= (GetFastAttack(KNIGHT, params) & GetPieces(ANY_COLOR, KNIGHT)) 
        |= (GetFastAttack(BISHOP, params) & GetPieces(ANY_COLOR, BISHOP)) 
        |= (GetFastAttack(KING, params) & GetPieces(ANY_COLOR, KING));

    return attacks;
}

void PositionAttacks::UpdateAttacks() noexcept 
{
    ComputePins();
    ComputeAttacks();
}

void PositionAttacks::ComputePins() noexcept 
{
    this->pinned = Bitboard::Null();

    const Color opp = side.opp();
    const Square ksq = GetPieces(side, KING).lsb();

    AttackParams attack_params;
    attack_params
        .set_blockers(Bitboard::Null())
        .set_attacker(ksq);

    Bitboard snipers = 
        GetFastAttack(ROOK, attack_params) & GetPieces(opp, ROOK, QUEEN) |
        GetFastAttack(BISHOP, attack_params) & GetPieces(opp, BISHOP, QUEEN);
    Bitboard occ = GetOccupied(WHITE, BLACK) ^ snipers;

    while(snipers)
    {
        Square sniper = snipers.poplsb();
        Bitboard b = between(ksq, sniper) & occ;

        if(b.count() == 1 && b & GetOccupied(side)) 
            this->pinned |= b;
    }
}

void PositionAttacks::ComputeAttacks() noexcept 
{
    this->attackers = Bitboard::Null();
    this->checkers = Bitboard::Null();
    this->defense = Bitboard::Full();

    const Color opp = side.opp();

    Bitboard    pawns   = GetPieces(opp, PAWN);
    Bitboard    pieces  = GetOccupied(opp) ^ pawns;
    Bitboard    king    = GetPieces(side, KING);
    Square      ksq     = king.lsb();

    AttackParams attack_params;
    attack_params.set_blockers(GetOccupied(WHITE, BLACK) ^ king);

    while(pieces)
    {
        Square  from = pieces.poplsb();
        Piece   type = GetPiece(from);
        attack_params.set_attacker(from);
        
        Bitboard piece_attacks = GetFastAttack(type, attack_params);
        this->attackers |= piece_attacks;

        if(piece_attacks & king) {
            this->checkers  |=  from.bitboard();
            this->defense   &=  GetPiece(from).is(KNIGHT) 
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
}


template<StorageType Policy>
Position<Policy>::Position()
{
    Setup();
}

template <StorageType Policy>
Position<Policy>::Position(std::string_view fen) noexcept 
{
    Setup();
    SetFen(fen);
}

template<StorageType Policy>
void Position<Policy>::SetFen(std::string_view fen) noexcept 
{
    State& new_st = st.create();

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
            case 'K': AddPiece(WHITE, KING,    sqr, new_st.hash);   break;
            case 'Q': AddPiece(WHITE, QUEEN,   sqr, new_st.hash);   break;
            case 'P': AddPiece(WHITE, PAWN,    sqr, new_st.hash);   break;
            case 'N': AddPiece(WHITE, KNIGHT,  sqr, new_st.hash);   break;
            case 'R': AddPiece(WHITE, ROOK,    sqr, new_st.hash);   break;
            case 'B': AddPiece(WHITE, BISHOP,  sqr, new_st.hash);   break;
            case 'k': AddPiece(BLACK, KING,    sqr, new_st.hash);   break;
            case 'q': AddPiece(BLACK, QUEEN,   sqr, new_st.hash);   break;
            case 'p': AddPiece(BLACK, PAWN,    sqr, new_st.hash);   break;
            case 'n': AddPiece(BLACK, KNIGHT,  sqr, new_st.hash);   break;
            case 'r': AddPiece(BLACK, ROOK,    sqr, new_st.hash);   break;
            case 'b': AddPiece(BLACK, BISHOP,  sqr, new_st.hash);   break;
            default: break;
            }

            col++;
        }
    }

    side.set(clr);
    if(clr == 'w')
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

template<StorageType Policy>
std::string Position<Policy>::GetFen() const noexcept 
{ 
    std::ostringstream res;

    for(int rank = 7; rank >= 0; --rank)
    {
        for(int file = 0; file < 8; ++file)
        {
            Square sqr = Square::ToSquare(rank * 8 + file);

            int empty = 0;
            while(GetPiece(sqr) == NO_PIECE && file < 8)
            {
                file++;
                sqr = Square::ToSquare(rank * 8 + file);
                ++empty;
            }

            if(empty) res << empty;
            if(file == 8) continue;

            Bitboard sqr_bb = sqr.bitboard();

            switch (GetPiece(sqr))
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

    const State& curr_st = st.back();
    
    if(curr_st.castle != NO_CASTLING) {
        if(curr_st.castle.available(K_CASTLING)) res << 'K';
        if(curr_st.castle.available(k_CASTLING)) res << 'k';
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
void Position<Policy>::DoMove(Move move) noexcept 
{
    const Square passant = st.back().passant;

    State& new_st   =   st.create();
    new_st.move     =   move;

    if(passant.isValid()) 
        new_st.hash.updateEnPassant(passant);

    Square from     =   move.from();
    Square targ     =   move.targ();
    MoveFlag flag   =   move.flag();

    switch (flag)
    {
    case DEFAULT_MF:
        if(types[from].is(KING)) UpdateCastle(side, BOTH_SIDES_CASTLING);
        else TryToUpdateCastle(side, from);
        break;
    case S_CASTLE_MF:
        MovePiece(targ + EAST, from + EAST, new_st.hash);
        UpdateCastle(side, BOTH_SIDES_CASTLING);
        break;
    case L_CASTLE_MF:
        MovePiece(targ + 2 * WEST, from + WEST, new_st.hash);
        UpdateCastle(side, BOTH_SIDES_CASTLING);
        break;
    case DOUBLE_MF:
        UpdatePassant(where_passant(from, targ));
        break;
    case EN_PASSANT_MF:
        RemovePiece(side.opp(), where_passant(from, targ), new_st.hash);
        break;
    case Q_PROMOTION_MF:
        ReplacePiece(QUEEN, from, new_st.hash);
        break;
    case R_PROMOTION_MF:
        ReplacePiece(ROOK, from, new_st.hash);
        break;
    case K_PROMOTION_MF:
        ReplacePiece(KNIGHT, from, new_st.hash);
        break;
    case B_PROMOTION_MF:
        ReplacePiece(BISHOP, from, new_st.hash);
        break;
    }

    if(types[targ].isValid()) {
        TryToUpdateCastle(side.opp(), targ); // capture opp rook
        new_st.captured = types[targ];
        RemovePiece(side.opp(), targ, new_st.hash);
        new_st.rule50 = 0;
    } else if(!types[from].is(PAWN)) {
        new_st.rule50++;
    } else new_st.rule50 = 0;

    MovePiece(from, targ, new_st.hash);

    side.swap();
    new_st.hash.updateSide();
}

template<StorageType Policy>
void Position<Policy>::UndoMove() noexcept 
{
    const State& old_st = st.back();
    const Move last_move = old_st.move;
    const Piece captured = old_st.captured;
    
    const Square from = last_move.from();
    const Square targ = last_move.targ();
    const MoveFlag flag = last_move.flag();

    st.rollback();
    side.swap();

    switch (flag)
    {
    case S_CASTLE_MF:
        MovePiece(from + EAST, targ + EAST);
        break;
    case L_CASTLE_MF:
        MovePiece(from + WEST, targ + 2 * WEST);
        break;
    case EN_PASSANT_MF:
        AddPiece(side.opp(), PAWN, where_passant(from, targ));
        break;
    case Q_PROMOTION_MF:
    case R_PROMOTION_MF:
    case K_PROMOTION_MF:
    case B_PROMOTION_MF:
        ReplacePiece(PAWN, targ);
        break;
    default:
        break;
    }

    MovePiece(targ, from);

    if(captured.isValid()) {
        AddPiece(side.opp(), captured, targ);
    }
}

template<StorageType Policy>
bool Position<Policy>::IsDraw() const noexcept 
{ 
    return 
        st.back().rule50 == 50 || 
        NotEnoughPieces() ||
        st.hasRepeated();
}

template<StorageType Policy>
bool Position<Policy>::CanCastle(CastleType ct) const noexcept 
{ 
    const Castle cr = st.back().castle.extract(side, ct);
    return 
          cr.has_path()                     &&
        !(cr.king_path()  & attackers)  &&
        !(cr.clear_path() & GetOccupied(WHITE, BLACK));
}

template<StorageType Policy>
void Position<Policy>::UpdatePassant(Square sqr) noexcept {
    State &curr_st = st.back();
    curr_st.hash.updateEnPassant(sqr);
    curr_st.passant = sqr;
}

template<StorageType Policy>
void Position<Policy>::UpdateCastle(Color c, CastleType ct) noexcept 
{
    State& curr_st = st.back();
    const CastleRightsType cr = curr_st.castle.extract(c, ct);

    if(cr == NO_CASTLING) 
        return;

    curr_st.hash.updateCastle(curr_st.castle);
    curr_st.castle.rem(cr);
    curr_st.hash.updateCastle(curr_st.castle);
}

template<StorageType Policy>
void Position<Policy>::TryToUpdateCastle(Color c, Square maybe_rook) noexcept 
{
    if(types[maybe_rook].is(ROOK)) {
        if(std::optional ct = Castle::ByRookSquare(c, maybe_rook)) {
            UpdateCastle(c, *ct);
        }
    }
}

template<StorageType Policy>
bool Position<Policy>::NotEnoughPieces() const noexcept 
{ 
    int total_pieces = GetOccupied(WHITE, BLACK).count();
    return (
        total_pieces == 2 ||  // 2 kings
        total_pieces == 3 && ( // 1 king vs king + bishop / knight
            GetPieces(WHITE, BISHOP, KNIGHT).count() == 1 ||
            GetPieces(BLACK, BISHOP, KNIGHT).count() == 1
        )
    );
}

template class Position<StaticStorage>;
template class Position<DynamicStorage>;


} // namespace Core::Logic