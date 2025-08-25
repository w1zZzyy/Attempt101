#pragma once

#include "logic/move.hpp"
#include "logic/zobrist.hpp"
#include <optional>

namespace game::engine 
{

enum class EntryType : logic::U8 {
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
    void store(const logic::Zobrist& key, int score, int depth, logic::Move move, EntryType type);
    std::optional<int> probe(const logic::Zobrist& key, int depth, int alpha, int beta);
    std::string load_info() const;

private:

    TTEntry& find(const logic::Zobrist& key);
    void clear() noexcept;

private:

    TTEntry* data;
    size_t size;

};

}