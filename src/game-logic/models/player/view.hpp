#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

namespace game::model
{

    
class IPlayer {
    using json = boost::json::object;
public:
    virtual void update(const json&) = 0;
    virtual boost::asio::awaitable<json> await_message() = 0;
};
using PlayerPtr = std::unique_ptr<IPlayer>;


class HumanPlayer : public IPlayer {
    using json = boost::json::object;
public:
    static PlayerPtr Create(boost::asio::io_context& io) {
        return PlayerPtr(new HumanPlayer(io));
    }
    void update(const json&) override;
    boost::asio::awaitable<json> await_message() override;
private:
    HumanPlayer(boost::asio::io_context& io);
    boost::beast::websocket::stream<boost::beast::tcp_stream> ws;
};


// maybe engine observer


}