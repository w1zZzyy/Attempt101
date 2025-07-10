#pragma once 

#include "position.hpp"
#include "move.hpp"
#include "movelist.hpp"

#include "boost/json.hpp"

#include <atomic>
#include <memory>
#include <expected>
#include <vector>
#include <string>


namespace game
{
namespace handler
{


enum class GameResult
{
    White, Black,
    Draw, 
    NotStated, InProgress,
    Finished
};


class GameState;
using GamePtr = std::shared_ptr<GameState>;


class GameState : public std::enable_shared_from_this<GameState>
{
public:

    GameState();

    // return move if it is correct and plays it
    virtual std::expected<logic::Move, std::string> DoMove(
        std::string_view notation
    );

    virtual void getMoveUpdates(boost::json::object&, logic::Move) const;

    virtual GamePtr getPtr() {
        return shared_from_this();
    }

    GameResult getResult() const noexcept {return result;}
    void setResult(GameResult res) noexcept {result = res;}

protected:

    logic::Move PermutateMove(std::string_view notation) const;

    std::expected<logic::Move, std::string> FindMove(
        std::string_view notation
    ) const;

    void DoMove(logic::Move move);


    logic::Position         position;
    logic::MoveList         moves;
    std::atomic<GameResult> result = GameResult::NotStated;

};


}
}

