#pragma once 

#include "eval.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"
#include "logic/storage.hpp"

namespace game::engine
{

class Search {
public:

    Search() {Evaluation::Setup();}
    Search& SetGlobalHistory(const logic::StateStorage<logic::DynamicStorage>& gh) noexcept;
    Search& SetMaxDepth(int) noexcept;
    std::optional<logic::Move> FindBestMove(PositionFixedMemory&);

private:

    int negamax(PositionFixedMemory&, int depth, int alpha, int beta);
    int qsearch(PositionFixedMemory&, int alpha, int beta);

    template<logic::MoveGenType T>
    std::optional<int> generateMoves(PositionFixedMemory&, logic::MoveList&) const;
    
    enum class SearchType {Negamax, QSearch};

    template<SearchType T>
    int go(PositionFixedMemory&, int alpha, int beta, int depth);

    template<SearchType T>
    int searchRoute(PositionFixedMemory&, logic::Move, const Evaluation::Data&, int alpha, int beta, int depth);

private:

    long long nodes;
    int maxDepth;
    Evaluation eval;
    const logic::StateStorage<logic::DynamicStorage>* globalHistory;

};

}