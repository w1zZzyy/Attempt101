#pragma once 

#include "eval.hpp"
#include "tt.hpp"
#include "timer.hpp"

#include "logic/move.hpp"
#include "logic/storage.hpp"
#include <cstddef>
#include <cstdint>

namespace Core::Engine
{

class Search {
public:

    Search();

    std::optional<Logic::Move> find_move(Logic::PositionFM&);

public:

    struct Info {
        long long nodes;
        long long tt_cuts;
    };
    struct Options {
        uint64_t timeSec;
        uint64_t ttSizeMB;
    };

private:

    int negamax(Logic::PositionFM&, int depth, int alpha, int beta);
    int qsearch(Logic::PositionFM&, int alpha, int beta);

private:
    
    Info info;
    Timer timer;
    Transposition tt;
    Evaluation eval;
    

};

}