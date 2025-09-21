#include "storage.hpp"
#include "zobrist.hpp"
#include <cassert>

namespace Core::Logic
{

constexpr void stcopy(State &dst, const State &src)
{
    dst.hash = src.hash;
    dst.castle = src.castle;
    dst.rule50 = src.rule50;

    dst.move = {};
    dst.passant = NO_SQUARE;
    dst.captured = NO_PIECE;
}

constexpr bool compareStates(
    const Zobrist& lkey, const Zobrist& rkey, 
    int l50, int r50, int d,
    int& cnt
) {
    if(lkey == rkey) {
        cnt++;
        if(cnt == MAX_REPETITIONS)
            return true;
    }

    else if(!lkey)
        return true;

    else if(l50 != r50 - d) 
        return true;
        
    return false;
}

constexpr int countRepetitions(const State* curr, const State* begin, const Zobrist key) 
{
    assert(curr && begin);

    const int rule50 = curr->rule50;
    int cnt = 0;

    for(const State* hist = curr; hist >= begin; hist--) 
        if(compareStates(hist->hash, key, curr->rule50, rule50, (curr - hist), cnt))
            break;

    return cnt;
}

constexpr int countRepetitions(const State* curr, const State* begin)
{
    assert(curr && begin);

    const Zobrist& key = curr->hash;
    const int rule50 = curr->rule50;
    int cnt = 1;

    for(const State* hist = curr - 2; hist >= begin; hist--) 
        if(compareStates(hist->hash, key, curr->rule50, rule50, (curr - hist), cnt))
            break;

    return cnt;
}

#define ASSERT_MSG(expr, msg) \
    if(!(expr)) { std::cerr << "Assertion failed: " << msg << "\n"; assert(expr); }

State &StaticStorage::createImpl() noexcept
{
    State* next = curr + 1;
    assert(next < history + MAX_HISTORY_SIZE);

    stcopy(*next, *curr);
    curr = next;

    return *curr;
}

State &StaticStorage::rollbackImpl() noexcept
{
    assert(curr > history);
    --curr;
    return *curr;
}

State &StaticStorage::backImpl() noexcept
{
    assert(curr > history);
    return *curr;
}

const State &StaticStorage::backImpl() const noexcept
{
    assert(curr > history);
    return *curr;
}

int StaticStorage::countRepetitionsImpl(Zobrist key) const noexcept
{
    return countRepetitions(curr, history, key);
}

int StaticStorage::countRepetitionsImpl() const noexcept
{
    return countRepetitions(curr, history);
}

State &DynamicStorage::createImpl() noexcept 
{
    State& next = history.emplace_back();
    const int size = history.size();

    if(size == 1)
        return next;

    stcopy(next, history[size - 2]);
    return next;
}

State &DynamicStorage::rollbackImpl() noexcept 
{
    assert(history.size() > 1);
    history.pop_back();
    return history.back();
}

State &DynamicStorage::frontImpl() noexcept  
{
    assert(!history.empty());
    return history[0];
}

const State &DynamicStorage::frontImpl() const noexcept 
{
    assert(!history.empty());
    return history[0];
}

State &DynamicStorage::backImpl() noexcept 
{
    assert(!history.empty());
    return history.back();
}

const State &DynamicStorage::backImpl() const noexcept 
{
    assert(!history.empty());
    return history.back();
}

int DynamicStorage::countRepetitionsImpl(Zobrist key) const noexcept 
{
    return countRepetitions(&history.back(), history.data(), key);
}

int DynamicStorage::countRepetitionsImpl() const noexcept 
{
    return countRepetitions(&history.back(), history.data());
}

} // namespace Core::Logic