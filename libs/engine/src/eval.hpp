#pragma once

#include "position.hpp"

namespace game::engine
{

class Eval
{
public:

    static void Setup();
    
    void init(const PositionFixedMemory& pos);
    void update(const PositionFixedMemory& pos, logic::Move move);
    void rollback(const PositionFixedMemory& pos, logic::Move move);

    /* Eval& set_mg(logic::Color c, int val) noexcept {mg[c] = val; return *this;}
    Eval& set_eg(logic::Color c, int val) noexcept {eg[c] = val; return *this;}
    Eval& set_phase(int phase) noexcept {game_phase = phase; return *this;}

    int get_mg(logic::Color c) const noexcept {return mg[c];}
    int get_eg(logic::Color c) const noexcept {return eg[c];}
    int get_phase() const noexcept {return game_phase;} */

    int score(const PositionFixedMemory& pos) const;

private:

    void update_piece(logic::Color side, logic::Piece piece, logic::Square from, logic::Square targ);
    void add_piece(logic::Color side, logic::Piece piece, logic::Square sqr);
    void remove_piece(logic::Color side, logic::Piece piece, logic::Square from);
    void castle(logic::Color side, logic::Square kf, logic::Square kt, logic::Square rf, logic::Square rt);

private:

    int mg[logic::COLOR_COUNT];
    int eg[logic::COLOR_COUNT];
    int game_phase;

};


}