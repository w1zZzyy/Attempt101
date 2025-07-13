#pragma once

#include "position.hpp"

namespace game::logic
{


/* class IMoveList
{
public:

    virtual void generate(const Position&) = 0;
    virtual bool empty() const noexcept = 0;
    virtual size_t get_size() const noexcept = 0;
    
}; */


class DefaultMoveList
{
    inline static const size_t MAX_MOVES = 218;
public:

    void generate(const Position& p);
    bool empty() const noexcept {return size == 0;}
    size_t get_size() const noexcept {return size;}
    Move operator [] (size_t i) const {
        if(i >= size) 
            throw std::runtime_error("index out of range");
        return moves[i];
    }

private:

    Move moves[MAX_MOVES];
    size_t size = 0;


    void add(Square from, Square targ, MoveFlag flag) {
        moves[size] = Move(from, targ, flag); 
        ++size;
    }
    
    void piece_moves(const Position& p, const PositionParams& pp);
    void king_moves(const Position& p, const PositionParams& pp);

    template<ColorType Us>
    void pawn_moves(const Position& p, const PositionParams& pp);

    template<ColorType Us>
    void pinned_pawn_moves(Bitboard pawns, const Position& p, const PositionParams& pp);
    
    template<ColorType Us, bool Pinned>
    void en_passant_moves(Bitboard pawns, const Position& p, const PositionParams& pp);
    
    void pawn_move_generic(Bitboard moves, std::initializer_list<MoveFlag> flags, int offset_from);
};


}