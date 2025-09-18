#include "search.hpp"

#include "eval.hpp"
#include "pick.hpp"
#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"

#include <cassert>
#include <chrono>
#include <optional>

namespace game::engine
{

using namespace logic;

Search& Search::SetGlobalHistory(const StateStorage<DynamicStorage>& gh) noexcept {
    globalHistory = &gh;
    return *this;
}

Search& Search::SetMaxDepth(int depth) noexcept {
    maxDepth = depth;
    return *this;
}

std::optional<Move> Search::FindBestMove(PositionFixedMemory& pos) 
{
    auto start = std::chrono::high_resolution_clock::now();

    Move best_move;
    int best_score = -INF;
    nodes = 0;

    pos.update();
    eval.Init(pos);

    MoveList moves;
    if(std::optional score = generateMoves<MoveGenType::NotForced>(pos, moves))
        return score.value();

    auto eval_data = eval.GetData();

    for(auto move : moves) 
    {
        int score = searchRoute<SearchType::Negamax>(pos, move, eval_data, best_score, INF, maxDepth);

        if(score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::format(
        "---------------\n"
        "Time: {}\nNodes: {}\nScore: {}\n",
        std::chrono::duration<double>(end - start).count(), 
        nodes, 
        best_score  
    );

    return best_move;
}

int Search::negamax(PositionFixedMemory& pos, int depth, int alpha, int beta) 
{
    assert(depth >= 0);
    if(depth == 0) {
        nodes++;
        return qsearch(pos, alpha, beta);
    }
    return go<SearchType::Negamax>(pos, alpha, beta, depth);
}

int Search::qsearch(PositionFixedMemory& pos, int alpha, int beta)
{
    alpha = std::max(alpha, eval.Score());
    if(alpha >= beta) {
        nodes++;
        return alpha;
    }
    return go<SearchType::QSearch>(pos, alpha, beta, -1);
}

template<MoveGenType T>
std::optional<int> Search::generateMoves(PositionFixedMemory& pos, MoveList& moves) const 
{
    if(pos.is_draw(*globalHistory)) {
        return DRAW_SCORE;
    }

    moves.generate<T>(pos);

    if(moves.empty()) {
        if(pos.is_check()) return -INF + pos.get_ply();
        if constexpr (T == MoveGenType::Forced) return eval.Score();
        return DRAW_SCORE;
    }

    return std::nullopt;
}

template<Search::SearchType T>
int Search::searchRoute(PositionFixedMemory& pos, Move move, const Evaluation::Data& eval_data, int alpha, int beta, int depth) 
{
    int score;
    const Color us = pos.get_side();

    pos.do_move(move);
    eval.Update(move, us);

    if constexpr (T == SearchType::Negamax) score = -negamax(pos, depth - 1, -beta, -alpha);
    else score = -qsearch(pos, -beta, -alpha);

    eval.SetData(eval_data);
    pos.undo_move();

    return score;
}

template<Search::SearchType T>
int Search::go(PositionFixedMemory& pos, int alpha, int beta, int depth) 
{
    constexpr bool IsNegamax = (T == SearchType::Negamax);
    constexpr MoveGenType GenType = (IsNegamax ? MoveGenType::NotForced : MoveGenType::Forced);

    pos.update();

    MoveList moves;
    if(std::optional score = generateMoves<GenType>(pos, moves))
        return score.value();

    MovePicker order(moves, pos);

    auto eval_data = eval.GetData();

    while(std::optional move = order.next()) 
    {
        int score = searchRoute<T>(pos, *move, eval_data, alpha, beta, depth);

        if(score > alpha) {
            alpha = score;
            if(alpha >= beta) {
                return beta;
            }
        }
    }

    return alpha;
}

}