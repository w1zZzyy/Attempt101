#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

namespace game::model
{

    
class Player {
    using json = boost::json::object;
public:

    Player(boost::asio::io_context& io);

    void update(const json&);
    boost::asio::awaitable<json> await_message();

protected:
    boost::beast::websocket::stream<boost::beast::tcp_stream> ws;
    boost::asio::io_context::strand strand;
};



}