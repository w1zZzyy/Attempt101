#include "timer.hpp"

#include <chrono>


using namespace game::timer;


game::timer::Timer::Timer(
    net::io_context &io, 
    milliseconds time_limit) : 
    timer(io),
    time_left_ms(time_limit)
{}



net::awaitable<bool> game::timer::Timer::resume()
{
    start = std::chrono::steady_clock::now();
    timer.expires_after(time_left_ms);

    paused = false;

    try {
        co_await timer.async_wait(net::use_awaitable);
        time_left_ms = milliseconds(0);
        co_return true;
    }  catch(const boost::system::system_error& e) {
        time_left_ms -= TimePassed();
    }

    paused = true;

    co_return false;
}

void game::timer::Timer::pause()
{
    timer.cancel();
}

milliseconds game::timer::Timer::getTimeLeft() const
{
    if(paused) {
        return time_left_ms;
    }
    return time_left_ms - TimePassed();
}

milliseconds game::timer::Timer::TimePassed() const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        steady_clock::now() - start
    );
}
