#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <deque>
#include <memory>
#include <array>

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

class StateStorage
{
public:

    State& create();
    State& rollback();

    State& top();
    const State& top() const;

    bool repetition() const;

private:

    State history[MAX_HISTORY_SIZE];
    size_t size{0};

};


}