#pragma once 

#include "eval.hpp"
#include "tt.hpp"

#include "logic/move.hpp"
#include "logic/storage.hpp"
#include <cstddef>

namespace game::engine
{

class Search {
public:

    Search() {Evaluation::Setup();}
    Search& SetGlobalHistory(const logic::StateStorage<logic::DynamicStorage>& gh) noexcept;
    Search& SetMaxDepth(int) noexcept;
    Search& SetTTSizeMB(size_t);
    std::optional<logic::Move> FindBestMove(PositionFixedMemory&);

private:

    int negamax(PositionFixedMemory&, int depth, int alpha, int beta);
    int qsearch(PositionFixedMemory&, int alpha, int beta);

private:

    long long nodes;
    long long tt_cuts;
    int maxDepth;
    Evaluation eval;
    const logic::StateStorage<logic::DynamicStorage>* globalHistory;
    Transposition tt;

};

}