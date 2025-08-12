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

    Eval& set_mg(logic::Color c, int val) noexcept {mg[c] = val; return *this;}
    Eval& set_eg(logic::Color c, int val) noexcept {eg[c] = val; return *this;}
    Eval& set_phase(int phase) noexcept {game_phase = phase; return *this;}

    int get_mg(logic::Color c) const noexcept {return mg[c];}
    int get_eg(logic::Color c) const noexcept {return eg[c];}
    int get_phase() const noexcept {return game_phase;}

    int score(const PositionFixedMemory& pos) const;

private:

    int mg[logic::COLOR_COUNT];
    int eg[logic::COLOR_COUNT];
    int game_phase;

};


}