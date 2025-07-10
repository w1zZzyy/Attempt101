#pragma once

#include "ioman.hpp"
#include "game-logic/handler/game.hpp"
#include "response.hpp"

#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <optional>


namespace match
{


class Controller
{
public:

    net::awaitable<boost::json::object> handle_request();
    void send_response(match::handler::ResponsePtr resp);

private:

    boost::beast::flat_buffer buffer;
    boost::beast::websocket::stream<net::ip::tcp::socket> ws;
};


}