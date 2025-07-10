#pragma once

#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

namespace game::timer
{

namespace net = boost::asio;
using milliseconds = std::chrono::milliseconds;

class Timer
{
public:

    Timer(
        net::io_context& io, 
        milliseconds time_limit
    );

    net::awaitable<bool> resume();
    void pause();

    milliseconds getTimeLeft() const;

private:

    bool                                    paused = true;
    milliseconds                            time_left_ms;
    std::chrono::steady_clock::time_point   start;
    net::steady_timer                       timer;
    

    milliseconds TimePassed() const;
};


}

