#include "search.hpp"

#include "movelist.hpp"
#include <format>
#include <iostream>
#include <algorithm>

namespace game::engine
{

using namespace logic;

Search &Search::SetMaxDepth(int d)
{
    if(d >= MAX_HISTORY_SIZE) {
        throw std::runtime_error(std::format(
            "depth cant be more than {}\n", 
            MAX_HISTORY_SIZE
        ));
    }
    maxDepth = d;
    return *this;
}

Search &Search::StartSearchWorker(const std::function<void(RootMove)> &callback)
{
    search_thread = std::thread([&]()
    {
        while(!stop.load()) {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this](){return can_search.load();});

            if(stop.load()) 
                break;

            if(auto best_move = BestMove())
                callback(*best_move);

            can_search.store(false);
        }
    });

    return *this;
}

void Search::FindBestMove(const std::string &fen)
{
    if(can_search.load()) {
        std::cout << "wait for search to finish\n";
        return;
    }

    this->fen = fen;
    this->variations.clear();
    can_search.store(true);
    cv.notify_one();
}

void Search::Stop()
{
    stop.store(true);
    can_search.store(true);

    cv.notify_one();

    if(search_thread.joinable()) 
        search_thread.join();
}

std::optional<Search::RootMove> Search::BestMove()
{
    PositionFixedMemory pos(fen);
    MoveList moves;


    pos.compute_enemy_attackers().compute_pins_from_sliders();
    eval.init(pos);
    moves.generate(pos);


    if(moves.empty()) return std::nullopt;
    
    for(size_t i = 0; i < moves.get_size(); ++i) 
        variations.emplace_back(RootMove{moves[i], 0});

    for(size_t depth = 1; depth <= maxDepth; ++depth)
    {
        std::sort(variations.begin(), variations.end(), 
        [](const RootMove& r1, const RootMove& r2){ return r1.score > r2.score; });

        for(auto& var : variations) 
        {
            pos.do_move(var.move);
            eval.update(pos, var.move);

            var.score = -Negamax(pos, depth - 1, -INF, INF);

            eval.rollback(pos, var.move);
            pos.undo_move();
        }
    }


    return variations[0];
}

int Search::Negamax(PositionFixedMemory &pos, int depth, int alpha, int beta)
{
    if(depth == 0) 
        return eval.score(pos);

    pos.compute_enemy_attackers().compute_pins_from_sliders();

    MoveList moves;
    moves.generate(pos);

    if(moves.empty()) {
        return (
            pos.is_check() ? -INF + MAX_HISTORY_SIZE - depth :
            0
        );
    } 
    else if(pos.is_draw()) {
        return 0;
    }

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        int score = -Negamax(pos, depth - 1, -beta, -alpha);
        alpha = std::max(alpha, score);

        eval.rollback(pos, move);
        pos.undo_move();

        if(alpha >= beta) {
            return beta;
        }
    }

    return alpha;
}

}