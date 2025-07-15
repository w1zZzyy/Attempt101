#include "view.hpp"

#include <iostream>

namespace game::model 
{


HumanPlayer::HumanPlayer(boost::asio::io_context& io) : 
    ws(boost::asio::make_strand(io))
{}


void HumanPlayer::update(const boost::json::object& data)
{
    ws.async_write(
        boost::asio::buffer(data),
        [](boost::system::error_code ec, size_t bytes_transferred) {
            if(ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
            }
        }
    );
}


boost::asio::awaitable<HumanPlayer::json> HumanPlayer::await_message()
{
    boost::beast::flat_buffer buffer;
    
    co_await ws.async_read(
        buffer, 
        boost::asio::use_awaitable
    );

    std::string msg = boost::beast::buffers_to_string(buffer.data());

    co_return boost::json::parse(msg).as_object();
}


}