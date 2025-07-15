#include "view.hpp"

using namespace game::logic;

namespace game::model 
{
    
namespace 
{


std::optional<std::string_view> FindItem(const boost::json::object& msg, std::string_view name)
{
    auto iter = msg.find(name);
    if(iter == msg.end()) {
        return std::nullopt;
    }

    auto& v = iter->value();

    if(v.is_string()) 
    {
        auto& str = v.as_string();
        return std::string_view(
            str.data(),
            str.size()
        );
    }

    return std::nullopt;
}


}


Match::Match(PlayerPtr&& white, PlayerPtr&& black) : 
    players{std::move(white), std::move(black)}
{}


boost::asio::awaitable<void> Match::await_player(logic::Color side)
{
    boost::json::object msg = co_await players[side]->await_message();
    auto type = FindItem(msg, "type");
}


}