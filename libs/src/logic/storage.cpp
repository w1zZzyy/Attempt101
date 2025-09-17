#include "storage.hpp"
#include "zobrist.hpp"
#include <cassert>

namespace game::logic
{

void IStorage::stcopy(State &dst, const State &src)
{
    dst.hash = src.hash;
    dst.castle = src.castle;
    dst.rule50 = src.rule50;

    dst.move = {};
    dst.passant = NO_SQUARE;
    dst.captured = NO_PIECE;
}

int IStorage::state_repetions(const State *curr, const State* begin, Zobrist hash) const
{
    assert(curr);

    int counter = 0;
    const int rule50 = curr->rule50;

    for(const State* hist = curr; hist >= begin; hist -= 2) {
        if(hist->hash == hash) {
            counter++;
            if(counter == 3)
                return counter;
        }

        else if(hist->rule50 != rule50 - (curr - hist)) 
            return -1;
    }

    return counter;
}

State& StaticStorage::create() 
{
    State* next = curr + 1;
    assert(next < history + MAX_HISTORY_SIZE);

    stcopy(*next, *curr);
    curr = next;

    return *curr;
}

State& StaticStorage::rollback()
{
    assert(curr > history);
    --curr;
    return *curr;
}

int StaticStorage::count(Zobrist hash) const {
    return state_repetions(curr, history, hash);
}

bool StaticStorage::repetition() const
{
    return count(curr->hash) == 3;
}

State& DynamicStorage::create() 
{
    State& next = history.emplace_back();
    const int size = history.size();

    if(size == 1)
        return next;

    stcopy(next, history[size - 2]);
    return next;
}

State& DynamicStorage::rollback()
{
    assert(history.size() > 1);
    history.pop_back();
    return history.back();
}

int DynamicStorage::count(Zobrist hash) const {
    const State* back = &history.back();
    return state_repetions(back, history.data(), hash);
}


bool DynamicStorage::repetition() const
{
    const State* back = &history.back();
    return count(back->hash) == 3;
}

State *game::logic::DynamicStorage::begin()
{
    if(history.empty()) return nullptr;
    return &history[0];
}

}