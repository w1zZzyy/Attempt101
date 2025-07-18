#pragma once

#include "player/view.hpp"
#include "logic/src/position.hpp"
#include "logic/src/movelist.hpp"

#include <expected>

namespace game::model
{

enum class MatchResult {
    White, 
    Black,
    Draw,
    Continues
};


class Match;
using MatchPtr = std::shared_ptr<Match>;

class Match : std::enable_shared_from_this<Match> {
public:

    Match(Player&& white, Player&& black);
    std::expected<logic::Move, std::string> do_move(std::string_view notation);

    template<typename... Colors>
    void update(const boost::json::object& updates, Colors... clr) {
        (players[clr].update(updates), ...);
    }

    MatchResult get_result() const noexcept {return result;}
    const logic::DefaultMoveList& get_moves() const noexcept {return moves;}

    bool is_side_turn(logic::Color side) const noexcept {return side == position.get_side();}
    
private:

    void                            update_result();
    std::optional<logic::Move>      apply_move_if_valid(std::string_view notation);
    boost::asio::awaitable<void>    await_player(logic::Color side);

private:

    Player                  players[logic::Color::Count()];
    logic::Position         position;
    logic::DefaultMoveList  moves;
    MatchResult             result{MatchResult::Continues};
};




}