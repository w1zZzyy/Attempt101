#include "controller.hpp"

#include "boost/beast.hpp"

#include <optional>
#include <string_view>

using namespace match;

namespace beast = boost::beast;
namespace json = boost::json;


net::awaitable<json::object> match::Controller::handle_request()
{
    co_await ws.async_read(buffer, net::use_awaitable);

    std::string_view sv(static_cast<const char*>
        (buffer.data().data()),
        buffer.size()
    );

    auto val = json::value(sv);

    buffer.consume(buffer.size());

    co_return val.as_object();
}

void match::Controller::send_response(match::handler::ResponsePtr resp)
{
    ws.async_write(
        net::buffer(json::serialize(resp->get())),
        [this](boost::system::error_code, size_t){}
    );
}
