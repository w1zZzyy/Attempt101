#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace game::logic
{


/* struct State
{
    State() = default;
    State(const State& s) {copy_core(s);}

    void copy_core(const State& other) noexcept;

    Zobrist hash{};
    Castle castle{NO_CASTLING};
    int rule50{0};

    Move move{};
    Square passant{NO_SQUARE};
    Piece captured{NO_PIECE};
};


class IStateStorage {
public:

    // создаем новое состояние, равное предыдущему
    // и возврщаем ссылку на него для возможности модификации
    // если предыдущего состояния нету, то создаем состояние
    // с значениями по умолчанию
    virtual State& create() = 0;

    // удаляем последние состояние и возвращаем предыдущее
    virtual State& rollback() = 0;

    // - возвращает последние добавленное состояние
    // - (до окончания работы с обьектом 
    //   create() и rollback() лучше не вызывать)
    virtual State& top() = 0;
    virtual const State& top() const = 0;

    // 3x repetition or 50 move rule
    virtual bool is_draw() const = 0;

    virtual ~IStateStorage() = default;
};


// player vs player style
class DynamicStateStorage : public IStateStorage 
{
public:

    State& create() override;

    State& rollback() override;

    State& top() override;

    const State& top() const override;

    bool is_draw() const override;

private:

    std::deque<State> history;

};


using StateStoragePtr = std::unique_ptr<IStateStorage>; */



struct State
{
    Zobrist hash{};
    Castle castle{NO_CASTLING};
    int rule50{0};

    Move move{};
    Square passant{NO_SQUARE};
    Piece captured{NO_PIECE};
};

class IStateStorage {
protected:
    void stcopy(State& dst, const State& src);
    bool repetition_help(const State* curr, const State* begin) const;
};



// STATIC STORAGE (using stack)
class StaticStorage : public IStateStorage {
public:

    State& create();
    State& rollback();

    State& top() {return *curr;}
    const State& top() const {return *curr;}

    bool repetition() const {return repetition_help(curr, history);}

private:

    State history[MAX_HISTORY_SIZE];
    State* curr = history;

};



// DYNAMIC STORAGE (using heap)
class DynamicStorage : public IStateStorage {
public:

    State& create();
    State& rollback();

    State& top() {return history.back();}
    const State& top() const {return history.back();}

    bool repetition() const;

private:

    std::vector<State> history;

};



// STORAGE POLIC

template<typename T>
concept StorageType = std::is_base_of_v<IStateStorage, T>;

template<StorageType StoragePolicy>
class StateStorage : public StoragePolicy {
public:
    using StoragePolicy::create;
    using StoragePolicy::rollback;
    using StoragePolicy::top;
    using StoragePolicy::repetition;
};


}