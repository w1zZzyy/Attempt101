#pragma once

#include "position.hpp"

namespace game::logic
{


class MoveList
{
public:

    template<StorageType T>
    void generate(const Position<T>& p);
    bool empty() const noexcept {return size == 0;}
    size_t get_size() const noexcept {return size;}
    Move operator [] (size_t i) const {
        if(i >= size) 
            throw std::runtime_error("index out of range");
        return moves[i];
    }
    std::optional<Move> find(std::string_view notation) const;
    Move* data() noexcept {return moves;}

private:

    Move moves[MAX_MOVES_COUNT];
    size_t size;


    void add(Square from, Square targ, MoveFlag flag) {
        moves[size] = Move(from, targ, flag); 
        ++size;
    }
    
    template<StorageType T>
    void piece_moves(const Position<T>& pos);

    template<StorageType T>
    void king_moves(const Position<T>& pos);

    template<ColorType Us, StorageType T>
    void pawn_moves(const Position<T>& pos);

    template<ColorType Us, StorageType T>
    void pinned_pawn_moves(Bitboard pawns, const Position<T>& pos);
    
    template<ColorType Us, bool Pinned, StorageType T>
    void en_passant_moves(Bitboard pawns, const Position<T>& pos);
    
    void pawn_move_generic(Bitboard moves, std::initializer_list<MoveFlag> flags, int offset_from);
};


}