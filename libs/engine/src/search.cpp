#include "search.hpp"

#include "movelist.hpp"

namespace game::engine
{


SearchResults Search::start(const SearchOptions &options)
{
    logic::Position pos(options.fen);

    logic::MoveList moves; 
    moves.generate(pos);

    SearchResults res;
    res.score = -logic::INF;

    for(size_t i = 0; i < moves.get_size(); ++i)
    {
        pos.do_move(moves[i]);

        Eval eval;
        eval.init(pos);

        int score = -negamax(pos, options.depth - 1, eval);
        if(score > res.score) {
            res.score = score;
            res.move = moves[i];
        }

        pos.undo_move();
    }

    return res;
}


int Search::negamax(logic::Position &pos, int depth, Eval& eval, int alpha, int beta)
{
    using namespace game::logic;

    if(depth == 0) 
        return eval.score(pos);

    MoveList moves;
    moves.generate(pos);

    if(moves.empty()) {
        return (
            pos.is_check() ? -INF + MAX_HISTORY_SIZE - depth :
            0
        );
    }

    for(size_t i = 0; i < moves.get_size(); ++i) 
    {
        pos.do_move(moves[i]);
        eval.update(pos, moves[i]);

        int mg[COLOR_COUNT] = {eval.get_mg(WHITE), eval.get_mg(BLACK)};
        int eg[COLOR_COUNT] = {eval.get_eg(WHITE), eval.get_eg(BLACK)};
        int phase = eval.get_phase();

        int score = -negamax(pos, depth - 1, eval, -beta, -alpha);
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