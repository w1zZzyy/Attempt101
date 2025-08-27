#include "logic.hpp"

namespace controller 
{

void GameLogic::Init(const std::string& fen) {
    pos.set_fen(fen);
    Update();
}

std::vector<game::logic::Move> GameLogic::MovesFrom(game::logic::Square from) const 
{
    std::vector<game::logic::Move> piece_moves;

    for(size_t i = 0; i < moves.get_size(); ++i) {
        if(moves[i].from() == from) 
            piece_moves.push_back(moves[i]);
    }
    
    return piece_moves;
}

game::logic::Move GameLogic::DoMove(game::logic::Move move) 
{
    pos.do_move(move);
    Update();
    return move;
}

void GameLogic::Update() 
{
    pos.update();
    moves.generate<game::logic::MoveGenType::NotForced>(pos);

    if(moves.empty()) {
        state = pos.is_check()
        ? static_cast<State>(int(pos.get_side().opp()))
        : State::Draw;
    } else if(pos.is_draw()) {
        state = State::Draw;
    } else {
        state = State::InProgress;
    }
}

}