#include "tt.hpp"
#include "logic/move.hpp"
#include <cstdint>
#include <cstring>
#include <optional>

namespace Core::Engine
{

struct TTEntry 
{
    uint16_t key;
    int16_t score;
    uint16_t move;
    uint8_t depth;
    EntryType flag;
};

constexpr int ClusterSize = 3;

struct Cluster {
    TTEntry entry[ClusterSize];
};

Transposition::~Transposition() {clear();}

void Transposition::resize(size_t mb) 
{
    clear();

    constexpr size_t bytes = sizeof(Cluster);
    size = mb * 1024 * 1024 / bytes;
    table = new Cluster[size];

    for (size_t i = 0; i < size; ++i)
        std::memset(&table[i], 0, bytes);
}

void Transposition::store(
    uint64_t key, int16_t score, Logic::Move move, uint8_t depth, EntryType flag
) {
    TTEntry* entry = first_entry(key);
    const uint16_t key16 = uint16_t(key);

    for (int i = 0; i < ClusterSize; ++i) {
        if(entry[i].key == key16) {
            if(flag == EntryType::Exact || depth >= entry[i].depth) 
            {
                entry[i].depth = depth;
                entry[i].score = score;
                entry[i].flag = flag;
                entry[i].move = move;
            }
            return;
        }
    }

    TTEntry* replace = entry;
    for (int i = 1; i < ClusterSize; ++i) 
        if(entry[i].depth < replace->depth) 
            replace = &entry[i];

    replace->key = key16;
    replace->depth = depth;
    replace->score = score;
    replace->flag = flag;
    replace->move = move;
}

ProbeResult Transposition::probe(uint64_t key, uint8_t depth, int alpha, int beta) const 
{
    TTEntry* entry = first_entry(key);
    const uint16_t key16 = uint16_t(key);

    for(int i = 0; i < ClusterSize; ++i)
    {
        if(entry[i].key == key16)
        {
            ProbeResult res;

            if(entry[i].depth >= depth)
            {
                switch (entry[i].flag) 
                {
                case EntryType::Exact:
                    res.score = entry[i].score;
                    break;
                case EntryType::LowerBound:
                    if(entry[i].score >= beta)
                        res.score = entry[i].score;
                    break;
                case EntryType::UpperBound:
                    if(entry[i].score <= alpha)
                        res.score = entry[i].score;
                    break;
                }
            }

            res.move = entry[i].move;

            return res;
        }
    }

    return {};
}


void Transposition::clear() 
{
    if(table) {
        delete[] table;
    }
    table = nullptr;
    size = 0;
}

TTEntry* Transposition::first_entry(uint64_t key) const
{
    __extension__ using uint128 = unsigned __int128;
    uint64_t index = ((uint128(key) * uint128(size)) >> 64);
    return &table[index].entry[0];
}

} 
