#pragma once

#include "logic/move.hpp"
#include "logic/zobrist.hpp"
#include <optional>

namespace game::engine 
{

enum class EntryType {
    Exact, 
    LowerBound,
    UpperBound
};

struct TTEntry
{
    int score;
    int depth;
    logic::Move move;
    EntryType type;
    logic::Zobrist key;
};

class Transpositions 
{
public:

    ~Transpositions() noexcept {clear();}
    void resize(size_t mb);
    void store(logic::Zobrist key, int score, int depth, logic::Move move, EntryType type);
    std::optional<int> probe(logic::Zobrist key, int depth, int alpha, int beta);

private:

    TTEntry& find(logic::Bitboard key);
    void clear() noexcept;

private:

    TTEntry* data;
    size_t size;

};

}