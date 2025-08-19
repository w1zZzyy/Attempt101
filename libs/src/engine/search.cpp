#include "search.hpp"

#include "logic/movelist.hpp"
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
    search_thread = std::thread([this, callback]()
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

    if(moves.empty()) 
        return std::nullopt;

    RootMove best;
    best.score = -INF;

    int mg[COLOR_COUNT] = {eval.get_mg(WHITE), eval.get_mg(BLACK)};
    int eg[COLOR_COUNT] = {eval.get_eg(WHITE), eval.get_eg(BLACK)};
    int phase = eval.get_phase();

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        auto move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        if(int score = -Negamax(pos, maxDepth - 1, -INF, INF); score > best.score) 
            best = {move, score};

        pos.undo_move();
        eval
            .set_mg(WHITE, mg[WHITE]) 
            .set_mg(BLACK, mg[BLACK]) 
            .set_eg(WHITE, eg[WHITE]) 
            .set_eg(BLACK, eg[BLACK]) 
            .set_phase(phase);
    }

    return best;
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

    int mg[COLOR_COUNT] = {eval.get_mg(WHITE), eval.get_mg(BLACK)};
    int eg[COLOR_COUNT] = {eval.get_eg(WHITE), eval.get_eg(BLACK)};
    int phase = eval.get_phase();

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        int score = -Negamax(pos, depth - 1, -beta, -alpha);
        alpha = std::max(alpha, score);

        pos.undo_move();
        eval
            .set_mg(WHITE, mg[WHITE]) 
            .set_mg(BLACK, mg[BLACK]) 
            .set_eg(WHITE, eg[WHITE]) 
            .set_eg(BLACK, eg[BLACK]) 
            .set_phase(phase);

        if(alpha >= beta) {
            return beta;
        }
    }

    return alpha;
}

}