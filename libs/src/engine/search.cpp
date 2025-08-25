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

Search& Search::SetTableSize(size_t mb) {
    tt.resize(mb);
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

            std::string info = "INFO:\n";

            if(std::optional node = BestMove()) {
                info += std::format("\tScore: {}\n\tMove: {}\n", node->score, node->move.to_string());
                callback(*node);
            }

            can_search.store(false);
            info += std::format("\tNodes Counted: {}\n", nodes);
            std::cout << info;
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

    eval.push();

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        Move move = moves[i];

        pos.do_move(move);
        eval.update(move);

        if(int score = -Negamax(pos, maxDepth - 1, best.score, INF); score > best.score) 
            best = {move, score};

        pos.undo_move();
        eval.rollback();
    }

    eval.pop();

    return best;
}

int Search::Negamax(PositionFixedMemory &pos, int depth, int alpha, int beta)
{
    if(std::optional ttVal = tt.probe(pos.get_hash(), depth, alpha, beta)) {
        std::cout << "ttVal: " << ttVal.value() << '\n';
        return ttVal.value();
    }

    if(depth == 0) 
        return QSearch(pos, alpha, beta);

    auto [move, score] = SearchMoves<MoveGenType::NotForced>(pos, alpha, beta, 
        [depth, this](PositionFixedMemory &pos, int a, int b) {
        return -Negamax(pos, depth - 1, -b, -a);
    });

    if (score < alpha) 
        tt.store(pos.get_hash(), score, depth, move, EntryType::UpperBound);
    else if(score > beta) 
        tt.store(pos.get_hash(), score, depth, move, EntryType::LowerBound);
    else 
        tt.store(pos.get_hash(), score, depth, move, EntryType::Exact);

    return score;
}

int Search::QSearch(PositionFixedMemory &pos, int alpha, int beta)
{
    alpha = std::max(alpha, eval.score());
    if(alpha >= beta) 
        return alpha;

    auto [_, score] = SearchMoves<MoveGenType::Forced>(pos, alpha, beta, 
        [this](PositionFixedMemory &pos, int a, int b) {
        return -QSearch(pos, -b, -a);
    });

    return score;
}

}