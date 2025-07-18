#include "storage.hpp"

using namespace game::logic;

void game::logic::State::copy_core(const State& other) noexcept
{
    hash = other.hash;
    castle = other.castle;
    rule50 = other.rule50;
    move = {};
    passant = NO_SQUARE;
    captured = NO_PIECE;
}

State &game::logic::DynamicStateStorage::create() {
    if (history.empty()) history.emplace_back();
    else history.emplace_back(history.back());
    return history.back();
}

State &game::logic::DynamicStateStorage::rollback() {
    history.pop_back();
    return history.back();
}

State &game::logic::DynamicStateStorage::top() {
    return history.back();
}

const State &game::logic::DynamicStateStorage::top() const {
    return history.back();
}

bool DynamicStateStorage::is_draw() const {
    if(history.empty() || history.back().rule50 != 50) {
        return false;
    }

    const Zobrist& h = history.back().hash;
    int counter = 1;
    int curr = history.size() - 2;

    while(curr >= 0 && history[curr].rule50 - 1 != history[curr + 1].rule50) {
        if(history[curr].hash == h) 
            ++counter;
        if(counter == 3) 
            return true;
        --curr;
    }

    return false;
}