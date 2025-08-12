#pragma once

#include "eval.hpp"
#include <string>

namespace game::engine
{

struct SearchOptions
{
    std::string fen;
    int depth;
};

struct SearchResults
{
    logic::Move move;
    int score;
};

class Search
{
public:

    SearchResults start(const SearchOptions& options);

private:

    int negamax(
        PositionFixedMemory& pos, 
        int depth, Eval& eval, 
        int alpha = -logic::INF, int beta = logic::INF
    );

};


}