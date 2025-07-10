#pragma once

#include "zobrist.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "storage.hpp"

#include <string_view>
#include <deque>
#include <functional>

namespace game::logic
{


class Position
{
public:

    Position();
    Position(std::string_view fen, StateStoragePtr&& st);

    void do_move(Move move);
    void undo_move();

    Piece piece_on(Square s) const {return types[s];}

    template<typename... Pieces>
    Bitboard get_pieces(Color c, Pieces... p) const {
        static_assert(std::is_same_v<Pieces, Piece> && ...);
        return (pieces[c][p] | ...);
    }
    template<typename... Colors>
    Bitboard get_pieces(Colors... c) const {
        static_assert(std::is_same_v<Colors, Color> && ...);
        return (occupied[c] | ...);
    }
    Color get_side() const noexcept {return side;}

    bool can_castle(CastleType ct, Bitboard enemy_attacks) const;

private:

    Bitboard pieces[Color::Count()][Piece::Count()];
    Bitboard occupied[Color::Count()];
    Piece types[Square::Count()];
    Color side;
    StateStoragePtr st;
    

    template<bool HashUpdate = true>
    void add_piece(Color color, Piece piece, Square sqr);

    template<bool HashUpdate = true>
    void remove_piece(Color color, Square sqr);
    
    template<bool HashUpdate = true>
    void move_piece(Square from, Square targ);

    template<bool HashUpdate = true>
    void replace(Piece new_p, Square s);
    
    void update_castle(Color c, CastleType ct);
    void try_to_update_castle(Color c, Square maybe_rook);
};


template <bool HashUpdate>
inline void Position::add_piece(Color color, Piece piece, Square sqr)
{
    auto new_piece = sqr.bitboard();

    pieces[color][piece] |= new_piece;
    occupied[color] |= new_piece;
    types[sqr] = piece;

    if constexpr (HashUpdate)
        st->top().hash.updateSquare(color, piece, sqr);
}

template <bool HashUpdate>
inline void Position::remove_piece(Color color, Square sqr)
{
    auto del_piece = sqr.bitboard();
    auto piece = types[sqr];
    
    pieces[color][piece] ^= del_piece;
    occupied[color] ^= del_piece;
    types[sqr] = NO_PIECE;

    if constexpr (HashUpdate)
        st->top().hash.updateSquare(color, piece, sqr);
}

template <bool HashUpdate>
inline void Position::move_piece(Square from, Square targ)
{
    auto piece = types[from];
    auto move_bb = from.bitboard() | targ.bitboard();

    pieces[side][piece] ^= move_bb;
	occupied[side] ^= move_bb;

    if constexpr (HashUpdate) {
        auto& curr_st = st->top();
        curr_st.hash.updateSquare(side, piece, from);
        curr_st.hash.updateSquare(side, piece, targ);
    }
}

template <bool HashUpdate>
inline void Position::replace(Piece new_p, Square s)
{
    auto piece_bb = s.bitboard();
    auto old_p = types[s];

    pieces[side][old_p] ^= piece_bb;
    types[s] = new_p;
    pieces[side][new_p] ^= piece_bb;

    if constexpr (HashUpdate) {
        auto& curr_st = st->top();
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

    Bitboard pinned_pieces() const noexcept {return pinned;}
    Bitboard pin_mask(Square sqr, const Position& p) const;
    Bitboard enemy_attacks() const noexcept {return attackers;}
    Bitboard defensive_squares() const noexcept {return defense;}

private:

    Bitboard attackers; 
    Bitboard checkers;
    Bitboard pinned;
    Bitboard defense{Bitboard::Full()};

};


}