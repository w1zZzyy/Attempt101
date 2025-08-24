#include "tt.hpp"

namespace game::engine 
{

void Transpositions::resize(size_t mb)
{
    clear();

    constexpr size_t entry_bytes = sizeof(TTEntry);
    size = mb * 1024 * 1024 / entry_bytes;
    data = new TTEntry[size];
}

void Transpositions::store(logic::Zobrist key, int score, int depth, logic::Move move, EntryType type)
{
    TTEntry& entry = find(key);
    if(depth > entry.depth) {
        entry.depth = depth;
        entry.move = move;
        entry.score = score;
        entry.key = key;
        entry.type = type;
    }
}

std::optional<int> Transpositions::probe(logic::Zobrist key, int depth, int alpha, int beta) const 
{
    TTEntry& entry = find(key);
    if(entry.depth >= depth && entry.key == key) 
    {
        switch (entry.type) 
        {
        case EntryType::Exact: 
            return entry.score;
        case EntryType::LowerBound:
            if(entry.score >= beta)
                return entry.score;
            break;
        case EntryType::UpperBound:
            if(entry.score <= alpha)
                return entry.score;
            break;
        }
    }
    return std::nullopt;
}

TTEntry& Transpositions::find(logic::Bitboard key) const {
    assert(data);
    size_t index = key % size;
    return data[index];
}

void Transpositions::clear() noexcept {
    size = 0;
    if(data) 
        delete [] data;
}

}