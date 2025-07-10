#pragma once

#include "game.hpp"
#include "timer.hpp"

namespace game
{
namespace handler
{


class GameStateTimed : public GameState
{
public:

    GameStateTimed(timer::net::io_context& io, timer::milliseconds time);
    ~GameStateTimed();

    void LaunchTimer();

    void getMoveUpdates(boost::json::object&, logic::Move) const override;
    
    std::expected<logic::Move, std::string> DoMove(std::string_view notation) override;

    GamePtr getPtr() override {
        return std::static_pointer_cast<GameStateTimed>(
            shared_from_this()
        );
    }

private:

    timer::net::awaitable<void> TimerWait(logic::ColorType side);


    timer::net::io_context& io;
    timer::Timer clock[logic::COLORS];

};


}
}