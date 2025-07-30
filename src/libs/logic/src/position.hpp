#pragma once

#include "square.hpp"
#include "zobrist.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "storage.hpp"
#include "defs.hpp"

#include <cassert>
#include <ostream>
#include <string_view>

namespace game::logic
{


class Position
{
public:
    static void Init();

    Position() = default;
    Position(std::string_view fen);

    Position& set_fen(std::string_view fen);

    void do_move(Move move);
    void undo_move();

    Piece piece_on(Square s) const {return types[s];}

    template<typename... Pieces>
    Bitboard get_pieces(Color c, Pieces... p) const {
        constexpr bool isPiece      =   (std::is_same_v<Pieces, Piece> && ...);
        constexpr bool isPieceType  =   (std::is_same_v<Pieces, PieceType> && ...);
        static_assert((isPiece || isPieceType), "All template arguments must be of type Piece or PieceType");
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

    bool can_castle(CastleType ct, Bitboard enemy_attacks) const;

    std::string fen() const noexcept;

    bool is_draw() const;

private:

    Bitboard pieces[COLOR_COUNT][PIECE_COUNT];
    Bitboard occupied[COLOR_COUNT];
    Piece types[SQUARE_COUNT];
    Color side;
    StateStorage st;
    

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
};


template <bool HashUpdate>
inline void Position::add_piece(Color color, Piece piece, Square sqr)
{
    Bitboard new_piece = sqr.bitboard();

    pieces[color][piece] |= new_piece;
    occupied[color] |= new_piece;
    types[sqr] = piece;

    if constexpr (HashUpdate)
        st.top().hash.updateSquare(color, piece, sqr);
}

template <bool HashUpdate>
inline void Position::remove_piece(Color color, Square sqr)
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

template <bool HashUpdate>
inline void Position::move_piece(Square from, Square targ)
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
        curr_st.hash.updateSquare(side, piece, from);
        curr_st.hash.updateSquare(side, piece, targ);
    }
}

template <bool HashUpdate>
inline void Position::replace(Piece new_p, Square s)
{
    Bitboard piece_bb = s.bitboard();
    Piece old_p = types[s];

    assert(old_p.isValid());

    pieces[side][old_p] ^= piece_bb;
    types[s] = new_p;
    pieces[side][new_p] ^= piece_bb;

    if constexpr (HashUpdate) {
        State& curr_st = st.top();
        curr_st.hash.updateSquare(side, old_p, s);
        curr_st.hash.updateSquare(side, new_p, s);
    }
}


class PositionParams
{
public:

    PositionParams& compute_enemy_attackers(const Position& p);
    PositionParams& compute_pins_from_sliders(const Position& p);

    bool is_check() const noexcept {return checkers;}
    bool is_double_check() const {return checkers.count() == 2;}

    template<typename... Squares>
    bool is_blocker(Squares... sqr) const noexcept;
    bool is_attacker(Square sqr) const noexcept {return checkers & sqr.bitboard();}

    // оч редкая ситуация (при взятии на проходе шах нашему королю например)
    // используется для взятия на проходе
    // пример: Белый король: е5, Белая пешка: f5, Черная пешка: g5, Черная ладья: h5, (взятие на g6)
    bool exposes_discovered_check(Square from, Square targ, const Position& pos) const;

    Bitboard pinned_pieces() const noexcept {return pinned;}
    Bitboard pin_mask(Square sqr, const Position& p) const;
    Bitboard enemy_attacks() const noexcept {return attackers;}
    Bitboard defensive_squares() const noexcept {return defense;}

private:

    Bitboard attackers; 
    Bitboard checkers;
    Bitboard pinned;
    Bitboard king_blockers;
    Bitboard defense{Bitboard::Full()};

};

template <typename... Squares>
inline bool PositionParams::is_blocker(Squares... sqr) const noexcept
{
    Bitboard blockers = Bitboard::FromSquares(sqr...);
    return (king_blockers & blockers) == blockers;
}

}


std::ostream& operator<<(std::ostream& out, const game::logic::Position& position);

