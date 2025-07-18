#include "view.hpp"

#include "handlers/request/view.hpp"
#include <iostream>

namespace game::model 
{

using namespace logic;


Match::Match(Player&& white, Player&& black) : 
    players{std::move(white), std::move(black)}
{}

std::expected<logic::Move, std::string> Match::do_move(std::string_view notation)
{
    if(std::optional<Move> move = apply_move_if_valid(notation)) {
        update_result();
        return move.value();
    }
    return std::unexpected("move notation is invalid");
}

std::optional<Move> Match::apply_move_if_valid(std::string_view notation)
{
    std::optional<Move> move = moves.find(notation);
    if(!move) 
        return std::nullopt;

    position.do_move(*move);
    moves.generate(position);

    return move;
}

void Match::update_result()
{
    if(moves.empty()) 
    {   
        PositionParams pp;
        pp.compute_enemy_attackers(position);

        if(pp.is_check()) 
        {
            int winner_side = position.get_side().opp();
            result = static_cast<MatchResult>(winner_side);
        } 

        else 
            result = MatchResult::Draw;
    }
    else if(position.is_draw())   
        result = MatchResult::Draw;
}


boost::asio::awaitable<void> Match::await_player(logic::Color side)
{
    for(;;)
    {
        if(result != MatchResult::Continues) 
            break;

        try {
            boost::json::object req = co_await players[side].await_message();
            handler::HandleRequest(shared_from_this(), side, req);
        } catch (boost::system::system_error& e) {
            std::cerr << "error in await_player: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cerr << "error in await_player: " << e.what() << '\n';
        }
    }
}


}