#pragma once 

#include "logic/movelist.hpp"

namespace controller 
{

class GameLogic 
{
public:

    enum class State { WhiteWin, BlackWin, Draw, InProgress};

public:

    void Init(const std::string&);
    void DoMove(game::logic::Move);
    std::vector<game::logic::Move> MovesFrom(game::logic::Square) const;
    operator const PositionDynamicMemory&() const noexcept {return pos;}

private: 

    void Update();

private:

    PositionDynamicMemory pos;
    game::logic::MoveList moves;
    State state;

};

}