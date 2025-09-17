#pragma once

#include "square.hpp"
#include "zobrist.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "storage.hpp"
#include "defs.hpp"
#include "attack.hpp"

#include <cassert>
#include <ostream>
#include <string_view>
#include <iostream>
#include <cstring>

namespace game::logic
{

template<StorageType Policy>
class Position
{
public:
    static void Init();

    Position() {Init();};
    Position(std::string_view fen);

    template<StorageType T>
    Position(const Position<T>& pos);

    Position& set_fen(std::string_view fen);

    void do_move(Move move);
    void undo_move();

    Piece piece_on(Square s) const {return types[s];}
    Color piece_clr_on(Square s) const {
        const Bitboard b = s.bitboard();
        return (
            (b & occupied[WHITE]) ? WHITE :
            (b & occupied[BLACK]) ? BLACK :
            ANY_COLOR
        );
    };

    template<typename... Pieces>
    Bitboard get_pieces(Color c, Pieces... p) const {
        constexpr bool isPiece      =   (std::is_same_v<Pieces, Piece> && ...);
        constexpr bool isPieceType  =   (std::is_same_v<Pieces, PieceType> && ...);
        static_assert((isPiece || isPieceType), "All template arguments must be of type Piece or PieceType");
        if(c == ANY_COLOR) 
            return Bitboard(((pieces[WHITE][p] | pieces[BLACK][p]) | ...));
        return Bitboard((pieces[c][p] | ...));
    }
    template<typename... Colors>
    Bitboard get_occupied(Colors... c) const {
        constexpr bool isColor = (std::is_same_v<Colors, Color> && ...);
        constexpr bool isColorType = (std::is_same_v<Colors, ColorType> && ...);
        static_assert((isColor || isColorType), "All template arguments must be of type Color or ColorType");
        return Bitboard((occupied[c] | ...));
    }
    Color get_side() const noexcept {return side;}
    Square get_passant() const {return st.top().passant;}
    Piece get_captured() const {return st.top().captured;}
    Bitboard get_attacks(Color attacker) const noexcept {return attackers[attacker];}
    Bitboard get_pinned(Color us) const noexcept {return pinned[us];}
    const Zobrist& get_hash() const {return st.top().hash;}
    size_t get_ply() const {return st.size();}
    const StateStorage<Policy>& get_history() const noexcept {return st;}

    bool can_castle(CastleType ct, Bitboard enemy_attacks) const;

    std::string fen() const noexcept;

    template<StorageType T>
    bool is_draw(const StateStorage<T>& globalHistory) const {
        if(not_enough_pieces()) return true;
        if(st.top().rule50 == 50) return true;
        return st.repetition(globalHistory);
    }
    bool is_draw() const {
        return not_enough_pieces() || st.top().rule50 == 50 || st.repetition();
    }

    bool is_check() const noexcept {return checkers;}
    bool is_double_check() const {return checkers.count() == 2;}

    template<typename... Squares>
    bool is_blocker(Color king_side, Squares... sqr) const noexcept;
    bool is_attacker(Square sqr) const noexcept {return checkers & sqr.bitboard();}
    
    Bitboard attacks_to(Square sqr, Bitboard occ) const;

    // оч редкая ситуация (при взятии на проходе шах нашему королю например)
    // используется для взятия на проходе
    // пример: Белый король: е5, Белая пешка: f5, Черная пешка: g5, Черная ладья: h5, (взятие на g6)
    bool exposes_discovered_check(Square from, Square targ) const;

    Bitboard pin_mask(Square sqr) const;
    Bitboard defensive_squares() const noexcept {return defense;}

    template<typename... Colors>
    void update(Colors... clr);

private:

    template<StorageType U> friend class Position;

    template<bool UpdateCheckers>
    Position& compute_attackers(Color attacker);
    Position& compute_pins(Color us);

    template<bool HashUpdate = true>
    void add_piece(Color color, Piece piece, Square sqr);

    template<bool HashUpdate = true>
    void remove_piece(Color color, Square sqr);
    
    template<bool HashUpdate = true>
    void move_piece(Square from, Square targ);

    template<bool HashUpdate = true>
    void replace(Piece new_p, Square s);
    
    void update_passant(Square sqr);
    void update_castle(Color c, CastleType ct);
    void try_to_update_castle(Color c, Square maybe_rook);

    bool not_enough_pieces() const noexcept;

private:

    Bitboard pieces[COLOR_COUNT][PIECE_COUNT];
    Bitboard occupied[COLOR_COUNT];
    Piece types[SQUARE_COUNT];
    Color side;
    StateStorage<Policy> st;

    Bitboard attackers[COLOR_COUNT]; 
    Bitboard pinned[COLOR_COUNT];
    Bitboard king_blockers[COLOR_COUNT];
    Bitboard checkers;
    Bitboard defense;

};

template<StorageType Policy>
template<StorageType T>
Position<Policy>::Position(const Position<T>& pos) 
{
    std::memcpy(pieces, pos.pieces, sizeof(Bitboard) * COLOR_COUNT * PIECE_COUNT);
    std::memcpy(occupied, pos.occupied, sizeof(Bitboard) * COLOR_COUNT);
    std::memcpy(types, pos.types, sizeof(Piece) * SQUARE_COUNT);

    side = pos.side;

    st.create() = pos.st.top();
}

template<StorageType Policy>
template <bool HashUpdate>
inline void Position<Policy>::add_piece(Color color, Piece piece, Square sqr)
{
    Bitboard new_piece = sqr.bitboard();

    pieces[color][piece] |= new_piece;
    occupied[color] |= new_piece;
    types[sqr] = piece;

    if constexpr (HashUpdate)
        st.top().hash.updateSquare(color, piece, sqr);
}

template<StorageType Policy>
template <bool HashUpdate>
inline void Position<Policy>::remove_piece(Color color, Square sqr)
{
    Bitboard del_b = sqr.bitboard();
    Piece piece = types[sqr];

    assert(piece.isValid());
    
    pieces[color][piece] ^= del_b;
    occupied[color] ^= del_b;
    types[sqr] = NO_PIECE;

    if constexpr (HashUpdate)
        st.top().hash.updateSquare(color, piece, sqr);
}

template<StorageType Policy>
template <bool HashUpdate>
inline void Position<Policy>::move_piece(Square from, Square targ)
{
    Piece piece = types[from];
    Bitboard move_bb = from.bitboard() | targ.bitboard();

    assert(piece.isValid());

    pieces[side][piece] ^= move_bb;
	occupied[side] ^= move_bb;
    types[from] = NO_PIECE;
    types[targ] = piece;

    if constexpr (HashUpdate) {
        State& curr_st = st.top();
        curr_st.hash
            .updateSquare(side, piece, from)
            .updateSquare(side, piece, targ);
    }
}

template<StorageType Policy>
template <bool HashUpdate>
inline void Position<Policy>::replace(Piece new_p, Square s)
{
    Bitboard piece_bb = s.bitboard();
    Piece old_p = types[s];

    assert(old_p.isValid());

    pieces[side][old_p] ^= piece_bb;
    types[s] = new_p;
    pieces[side][new_p] ^= piece_bb;

    if constexpr (HashUpdate) {
        State& curr_st = st.top();
        curr_st.hash
            .updateSquare(side, old_p, s)
            .updateSquare(side, new_p, s);
    }
}

template<StorageType Policy>
template <typename... Squares>
inline bool Position<Policy>::is_blocker(Color king_side, Squares... sqr) const noexcept
{
    Bitboard blockers = Bitboard::FromSquares(sqr...);
    return (king_blockers[king_side] & blockers) == blockers;
}

template<StorageType Policy>
template <typename... Colors>
inline void Position<Policy>::update(Colors... clr)
{
    if constexpr (sizeof...(clr) == 0) {
        compute_attackers<true>(side.opp());
        compute_pins(side);
        return;
    }

    assert(((clr == WHITE || clr == BLACK) && ...));

    (..., (
        clr == side 
            ? compute_attackers<true>(Color{clr}.opp())
            : compute_attackers<false>(Color{clr}.opp())
        ).compute_pins(clr)
    );
}

template<StorageType Policy>
template<bool UpdateCheckers>
Position<Policy>& Position<Policy>::compute_attackers(Color attacker)
{
    this->attackers[attacker] = Bitboard::Null();

    if constexpr (UpdateCheckers) {
        this->checkers = Bitboard::Null();
        this->defense = Bitboard::Full();
    }

    const Color deffender = attacker.opp();

    Bitboard    pawns   = get_pieces(attacker, PAWN);
    Bitboard    pieces  = get_occupied(attacker) ^ pawns;
    Bitboard    king    = get_pieces(deffender, KING);
    Square      ksq     = king.lsb();

    AttackParams attack_params;
    attack_params.set_blockers(get_occupied(WHITE, BLACK) ^ king);

    while(pieces)
    {
        Square  from = pieces.poplsb();
        Piece   type = piece_on(from);
        attack_params.set_attacker(from);
        
        Bitboard piece_attacks = GetFastAttack(type, attack_params);
        this->attackers[attacker] |= piece_attacks;

        if constexpr (UpdateCheckers) {
            if(piece_attacks & king) {
                this->checkers  |=  from.bitboard();
                this->defense   &=  piece_on(from).is(KNIGHT) 
                                    ? from.bitboard() 
                                    : between(ksq, from);
            }
        }
    }

    Bitboard pawn_attacks = (attacker.is(WHITE)) 
                            ? step<NORTH_EAST>(pawns) | step<NORTH_WEST>(pawns)
                            : step<SOUTH_EAST>(pawns) | step<SOUTH_WEST>(pawns);

    this->attackers[attacker] |= pawn_attacks;
    
    if constexpr (UpdateCheckers) {
        if (pawn_attacks & king) 
        {
            attack_params
                .set_color(deffender)
                .set_attacker(ksq);
                
            Bitboard pawn_checkers = GetFastAttack(PAWN, attack_params) & pawns;

            this->checkers  |=  pawn_checkers;
            this->defense   &=  pawn_checkers;
        }
    }

    assert(attackers[attacker] & king && checkers || !(attackers[attacker] & king) && !checkers);

    return *this;
}

}

template<game::logic::StorageType Policy>
std::ostream& operator<<(std::ostream& out, const game::logic::Position<Policy>& position)
{
    using namespace game::logic;

    constexpr char PieceName[COLOR_COUNT][PIECE_COUNT] =
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

using PositionFixedMemory = game::logic::Position<game::logic::StaticStorage>;
using PositionDynamicMemory = game::logic::Position<game::logic::DynamicStorage>;