#pragma once

#include "move.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <vector>

namespace Core::Logic
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


template<typename Derived>
class StateStorage {
public:

    State& create() noexcept {return cast()->createImpl();}
    State& rollback() noexcept {return cast()->rollbackImpl();}

    State& front() noexcept {return cast()->frontImpl();}
    const State& front() const noexcept {return cast()->frontImpl();}

    State& back() noexcept {return cast()->backImpl();}
    const State& back() const noexcept {return cast()->backImpl();}

    template<typename Parent>
    bool hasRepeated(const StateStorage<Parent>& globalHistory) const noexcept;
    bool hasRepeated() const noexcept {return cast()->countRepetitionsImpl() == MAX_REPETITIONS;}

    size_t size() const noexcept {return cast()->sizeImpl();}

private:

    constexpr Derived* cast() noexcept {return static_cast<Derived*>(this);}
    constexpr const Derived* cast() const noexcept {return static_cast<const Derived*>(this);}

    template<typename>
    friend class StateStorage;

};


template <typename Derived>
template <typename Parent>
inline bool StateStorage<Derived>::hasRepeated(const StateStorage<Parent> &globalHistory) const noexcept
{
    constexpr Derived* der = cast();
    constexpr Parent* par = globalHistory.cast();

    if(int cnt = der->countRepetitionsImpl()) 
    {
        if(cnt == MAX_REPETITIONS) 
            return true;

        const int cnt2 = par->countRepetitionsImpl(der->backImpl().hash);
        return cnt + cnt2 >= MAX_REPETITIONS;
    }

    return false;
}


class StaticStorage : public StateStorage<StaticStorage> {
protected:

    State& createImpl() noexcept;
    State& rollbackImpl() noexcept;

    State& frontImpl() noexcept {return *history;}
    const State& frontImpl() const noexcept {return *history;}

    State& backImpl() noexcept;
    const State& backImpl() const noexcept;

    int countRepetitionsImpl(Zobrist key) const noexcept;
    int countRepetitionsImpl() const noexcept;

    size_t sizeImpl() const noexcept {return curr - history;}

private:

    State history[MAX_HISTORY_SIZE];
    State* curr = history;
    friend class StateStorage<StaticStorage>;

};


class DynamicStorage : public StateStorage<DynamicStorage> {
protected:

    State& createImpl() noexcept;
    State& rollbackImpl() noexcept;

    State& frontImpl() noexcept;
    const State& frontImpl() const noexcept;

    State& backImpl() noexcept;
    const State& backImpl() const noexcept;

    int countRepetitionsImpl(Zobrist key) const noexcept;
    int countRepetitionsImpl() const noexcept;

    size_t sizeImpl() const noexcept {return history.size(); }

private:

    std::vector<State> history;
    friend class StateStorage<DynamicStorage>;

};


template<typename T>
concept StorageType = std::derived_from<T, StateStorage<T>>;


}