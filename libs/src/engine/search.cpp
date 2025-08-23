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

            if(std::optional best_move = BestMove())
                callback(*best_move);

            can_search.store(false);
            std::cout << "Nodes Counted: " << nodes << '\n';
        }
    });

    return *this;
}

void Search::FindBestMove(const std::string &fen_)
{
    if(can_search.load()) {
        std::cout << "wait for search to finish\n";
        return;
    }

    this->fen = fen_;
    this->nodes = 0;

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

    pos.update();
    eval.init(pos);
    moves.generate<MoveGenType::NotForced>(pos);
    orderer.setPosition(pos);

    if(moves.empty()) 
        return std::nullopt;

    RootMove best;
    best.score = -INF;

    Eval curr(eval);

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        if(int score = -Negamax(pos, maxDepth - 1, -INF, INF); score > best.score) 
            best = {move, score};

        pos.undo_move();
        eval = curr;
    }

    return best;
}

int Search::Negamax(PositionFixedMemory &pos, int depth, int alpha, int beta)
{
    if(depth == 0) 
        return QSearch(pos, alpha, beta);

    nodes++;
    pos.update();

    MoveList moves;
    moves.generate<MoveGenType::NotForced>(pos);

    if(moves.empty()) {
        if(pos.is_check())
            return -INF + MAX_HISTORY_SIZE - depth;
        return 0;
    } 
    else if(pos.is_draw()) {
        return 0;
    }

    Eval curr(eval);
    orderer.OrderCaptures(moves);

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(pos, move);

        int score = -Negamax(pos, depth - 1, -beta, -alpha);
        alpha = std::max(alpha, score);

        pos.undo_move();
        eval = curr;

        if(alpha >= beta) {
            return beta;
        }
    }

    return alpha;
}

int Search::QSearch(PositionFixedMemory &pos, int alpha, int beta)
{
    alpha = std::max(alpha, eval.score(pos));
    if(alpha >= beta) 
        return beta;

    nodes++;
    pos.update();

    MoveList moves;
    moves.generate<MoveGenType::Forced>(pos);

    Eval curr(eval);
    orderer.OrderCaptures(moves);

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];
    
        pos.do_move(move);
        eval.update(pos, move);

        int score = -QSearch(pos, -beta, -alpha);
        alpha = std::max(alpha, score);

        pos.undo_move();
        eval = curr;

        if(alpha >= beta) {
            return beta;
        }
    }

    return alpha;
}

}