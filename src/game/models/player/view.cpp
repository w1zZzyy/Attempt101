#include "view.hpp"

#include <iostream>

namespace game::model 
{


Player::Player(boost::asio::io_context& io) :
    strand(io),
    ws(io)
{}


void Player::update(const boost::json::object& data)
{
    ws.async_write(
        boost::asio::buffer(data),
        boost::asio::bind_executor(strand,
        [](boost::system::error_code ec, size_t) {
            if(ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
            }
        }
        )
    );
}


boost::asio::awaitable<Player::json> Player::await_message()
{
    co_await boost::asio::post(strand, boost::asio::use_awaitable);

    boost::beast::flat_buffer buffer;
    
    try {
        co_await ws.async_read(
            buffer, 
            boost::asio::use_awaitable
        );
    } catch (boost::system::system_error& e) {
        std::cerr << "error in await_message: " << e.what() << '\n';
        co_return json{};
    } catch (const std::exception& e) {
        std::cerr << "error in await_message: " << e.what() << '\n';
        co_return json{};
    }

    std::string msg = boost::beast::buffers_to_string(buffer.data());

    co_return boost::json::parse(msg).as_object();
}


}