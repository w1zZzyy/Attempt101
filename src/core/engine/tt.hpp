#pragma once

#include "logic/move.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>

namespace Core::Engine
{

enum class EntryType : uint8_t {
    Exact,
    LowerBound,
    UpperBound
};

struct TTEntry;
struct Cluster;

struct ProbeResult {
    std::optional<int16_t> score;
    std::optional<Logic::Move> move;
};

class Transposition {
public:

    ~Transposition();
    void resize(size_t);
    void store(uint64_t key, int16_t score, Logic::Move move, uint8_t depth, EntryType flag);
    ProbeResult probe(uint64_t key, uint8_t depth, int alpha, int beta) const;

private:

    void clear();
    TTEntry* first_entry(uint64_t key) const;

private:

    Cluster* table = nullptr;
    uint64_t size = 0;

};

} 
