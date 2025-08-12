#include "storage.hpp"

namespace game::logic
{

void IStateStorage::stcopy(State &dst, const State &src)
{
    dst.hash = src.hash;
    dst.castle = src.castle;
    dst.rule50 = src.rule50;

    dst.move = {};
    dst.passant = NO_SQUARE;
    dst.captured = NO_PIECE;
}

bool IStateStorage::repetition_help(const State *curr, const State* begin) const
{
    assert(curr);

    int counter = 1;
    const Zobrist& hash = curr->hash;
    const int rule50 = curr->rule50;

    for(const State* hist = curr - 2; hist >= begin; hist -= 2) {
        if(hist->hash == hash) {
            counter++;
            if(counter == 3)
                return true;
        }

        if(hist->rule50 != rule50 - (curr - hist)) 
            return false;
    }

    return false;
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

bool DynamicStorage::repetition() const
{
    const State* back = &history.back();
    return repetition_help(back, history.data());
}

}