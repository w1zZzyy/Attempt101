#include "game_timer.hpp"

using namespace game;
using namespace handler;
using namespace timer;
using namespace logic;

game::handler::GameStateTimed::GameStateTimed(
    timer::net::io_context &io, 
    timer::milliseconds time) : 
    GameState(),
    io(io),
    clock{Timer(io, time), Timer(io, time)}
{}

game::handler::GameStateTimed::~GameStateTimed()
{
    if(result == GameResult::InProgress) {
        result = GameResult::Finished;
    }
    
    clock[WHITE].pause();
    clock[BLACK].pause();
}

void game::handler::GameStateTimed::LaunchTimer()
{
    timer::net::co_spawn( 
        io, TimerWait(position.getSide2Move()), timer::net::detached
    );
}

void game::handler::GameStateTimed::getMoveUpdates(
    boost::json::object& upd, logic::Move move
) const {
    GameState::getMoveUpdates(upd, move);
    boost::json::object obj_clock;
    obj_clock["white"] = clock[logic::WHITE].getTimeLeft().count();
    obj_clock["black"] = clock[logic::BLACK].getTimeLeft().count();
    upd["clock"] = obj_clock;
}

std::expected<logic::Move, std::string> game::handler::GameStateTimed::DoMove(
    std::string_view notation)
{
    auto move = FindMove(notation);
    if (move) { 
        clock[position.getSide2Move()].pause();
        GameState::DoMove(move.value()); 
        LaunchTimer();
    } 
    return move;
}

timer::net::awaitable<void> game::handler::GameStateTimed::TimerWait(
    logic::ColorType side
) {
    bool expired = co_await clock[side].resume();
    if(expired && result.load() == GameResult::InProgress) 
    {
        auto opp = position.getBoard().getPiece(-side);
        result.store(popcount(opp) > 1
                ? static_cast<GameResult>(-side)
                : GameResult::Draw
        );
    }
}

