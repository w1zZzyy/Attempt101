#pragma once

#include <chrono>
#include <cstdint>

namespace game::engine
{

class Timer 
{
public:

    void setLimit(uint64_t l) noexcept {limit = l * 1000;}
    bool TimeUp() const noexcept;
    void Start() noexcept {start_time = now();}
    std::chrono::seconds TimePassed() const noexcept;

private:

    uint64_t now() const noexcept;

private:

    uint64_t start_time;
    uint64_t limit;

};

}