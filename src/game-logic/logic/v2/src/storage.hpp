#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <deque>
#include <stdexcept>
#include <memory>

namespace game::logic
{


struct State
{
    State() = default;
    State(const State& s) {copy_core(s);}

    void copy_core(const State& other) noexcept {
        hash = other.hash;
        castle = other.castle;
        rule50 = other.rule50;
        move = {};
        passant = NO_SQUARE;
        captured = NO_PIECE;
    }

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

    size_t curr = 0;
    std::deque<State> history;

};


// engine style
template<size_t Size>
class FixedStateStorage : public IStateStorage
{
public:

    State& create() override {
        if (curr + 1 >= Size) {
            throw std::runtime_error("buffer overflow");
        }

        history[curr + 1].copy_core_state(history[curr]);
        return history[++curr];
    }

    State& rollback() override {
        if (curr == 0) {
            throw std::runtime_error("cant rollback");
        }
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