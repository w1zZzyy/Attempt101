#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <vector>

namespace game::logic
{

// STORAGE POLICY

template<typename T>
concept StorageType = requires (T t, Zobrist key) {
    { t.create() };
    { t.rollback() };
    { t.top() };
    { t.repetition() };
    { t.count(key) };
    { t.size() };
    { t.begin() };
};
template<StorageType StoragePolicy>
class StateStorage : public StoragePolicy {
public:
    using StoragePolicy::create;
    using StoragePolicy::rollback;
    using StoragePolicy::top;
    using StoragePolicy::count;
    using StoragePolicy::repetition;
    using StoragePolicy::size;
    using StoragePolicy::begin;
};

struct State
{
    Zobrist hash{};
    Castle castle{NO_CASTLING};
    int rule50{0};

    Move move{};
    Square passant{NO_SQUARE};
    Piece captured{NO_PIECE};
};

class IStorage {
protected:
    void stcopy(State &dst, const State &src);
    int state_repetions(const State *curr, const State* begin, Zobrist hash) const;
    template<StorageType T, StorageType P>
    bool repetition_help(const T& curr, const P& global, Zobrist hash) const;
};

template<StorageType T, StorageType P>
bool IStorage::repetition_help(const T& curr, const P& global, Zobrist hash) const {
    int repetitions = curr.count(hash);
    if(repetitions != -1 && repetitions < 3) {
        repetitions += global.count(hash);
    }
    return repetitions == 3;
}

// STATIC STORAGE (using stack)
class StaticStorage : public IStorage {
public:

    State& create();
    State& rollback();
    State& top() {return *curr;}
    const State& top() const {return *curr;}
    int count(Zobrist hash) const;
    bool repetition() const;
    template<StorageType StoragePolicy>
    bool repetition(const StateStorage<StoragePolicy>& globalHistory) const;
    size_t size() const {return curr - history;}
    State* begin() {return history;}

private:

    State history[MAX_HISTORY_SIZE];
    State* curr = history;

};

template<StorageType StoragePolicy>
bool StaticStorage::repetition(const StateStorage<StoragePolicy>& _global) const {
    const StoragePolicy& global = static_cast<const StoragePolicy&>(_global);
    return repetition_help(*this, global, curr->hash);
}


// DYNAMIC STORAGE (using heap)
class DynamicStorage : public IStorage {
public:

    State& create();
    State& rollback();
    State& top() {return history.back();}
    const State& top() const {return history.back();}
    int count(Zobrist hash) const;
    bool repetition() const;
    template<StorageType StoragePolicy>
    bool repetition(const StateStorage<StoragePolicy>& globalHistory) const;
    size_t size() const {return history.size();}
    State* begin();

private:

    std::vector<State> history;

};

template<StorageType StoragePolicy>
bool DynamicStorage::repetition(const StateStorage<StoragePolicy>& _global) const {
    const StoragePolicy& global = static_cast<const StoragePolicy&>(_global);
    return repetition_help(*this, global, history.back().hash);
}


}