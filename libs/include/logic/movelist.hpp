#pragma once

#include "position.hpp"

namespace game::logic
{

enum class MoveGenType {NotForced, Forced};

class MoveList
{
public:

    template<MoveGenType MGT, StorageType ST>
    void generate(const Position<ST>& p);
    bool empty() const noexcept {return get_size() == 0;}
    size_t get_size() const noexcept {return curr - moves;}
    Move operator [] (size_t i) const {
        if(i >= get_size()) 
            throw std::runtime_error("index out of range");
        return moves[i];
    }
    Move* data() noexcept {return moves;}

private:

    Move moves[MAX_MOVES_COUNT];
    Move* curr = moves;

};


}