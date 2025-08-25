#pragma once

#include "logic/position.hpp"
#include <stack>

namespace game::engine
{

class Eval
{
public:

    static void Setup();
    
    void init(const PositionFixedMemory& pos);
    void update(logic::Move move);

    int score() const;
    
    void push() {prev.push(data);}
    void rollback() {assert(!prev.empty()); data = prev.top();}
    void pop() {assert(!prev.empty()); prev.pop();}

private:

    void update_piece(logic::Color side, logic::Piece piece, logic::Square from, logic::Square targ);
    void add_piece(logic::Color side, logic::Piece piece, logic::Square sqr);
    void remove_piece(logic::Color side, logic::Piece piece, logic::Square from);
    void castle(logic::Color side, logic::Square kf, logic::Square kt, logic::Square rf, logic::Square rt);

private:

    struct Data {
        int mg[logic::COLOR_COUNT];
        int eg[logic::COLOR_COUNT];
        int game_phase;
    };

private:

    const PositionFixedMemory* pos;
    Data data;
    std::stack<Data> prev;

};


}