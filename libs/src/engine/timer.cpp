#include "timer.hpp"

#include <chrono>
#include <cstdint>

namespace game::engine
{

bool Timer::TimeUp() const noexcept 
{
    return now() - start_time >= limit;
}

uint64_t Timer::now() const noexcept {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

std::chrono::seconds Timer::TimePassed() const noexcept {
    using namespace std::chrono;
    auto ms = static_cast<milliseconds>(now() - start_time);
    return duration_cast<seconds>(ms);
}


}