#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <deque>
#include <memory>

namespace game::logic
{


struct State
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
};


// player vs player style
class DynamicStateStorage : public IStateStorage 
{
public:

    State& create() override;

    State& rollback() override;

    State& top() override;

    const State& top() const override;

private:

    std::deque<State> history;

};


// engine style
template<size_t Size>
class FixedStateStorage : public IStateStorage
{
public:

    State& create() override {
        assert(curr + 1 < Size);
        history[curr + 1].copy_core(history[curr]);
        return history[++curr];
    }

    State& rollback() override {
        assert(curr != 0);
        return history[--curr];
    }

    State& top() override {
        return history[curr];
    }

    const State& top() const override {
        return history[curr];
    }

private:

    size_t curr = 0;
    State history[Size];

};


using StateStoragePtr = std::unique_ptr<IStateStorage>;


}