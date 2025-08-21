#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <vector>

namespace game::logic
{

struct State
{
    Zobrist hash{};
    Castle castle{NO_CASTLING};
    int rule50{0};

    Move move{};
    Square passant{NO_SQUARE};
    Piece captured{NO_PIECE};
};

// STATIC STORAGE (using stack)
class StaticStorage {
public:

    State& create();
    State& rollback();

    State& top() {return *curr;}
    const State& top() const {return *curr;}

    bool repetition() const;

private:

    State history[MAX_HISTORY_SIZE];
    State* curr = history;

};



// DYNAMIC STORAGE (using heap)
class DynamicStorage {
public:

    State& create();
    State& rollback();

    State& top() {return history.back();}
    const State& top() const {return history.back();}

    bool repetition() const;

private:

    std::vector<State> history;

};



// STORAGE POLICY

template<typename T>
concept StorageType = requires (T t) {
    { t.create() };
    { t.rollback() };
    { t.top() };
    { t.repetition() };
};

template<StorageType StoragePolicy>
class StateStorage : public StoragePolicy {
public:
    using StoragePolicy::create;
    using StoragePolicy::rollback;
    using StoragePolicy::top;
    using StoragePolicy::repetition;
};


}