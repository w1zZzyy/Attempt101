#pragma once

#include <userver/server/http/http_request.hpp>

#include <string>
#include <string_view>
#include <optional>

namespace user 
{

struct TUser
{
    long long   id;
    std::string name;
    std::string password;
    std::string status;
};


std::optional<TUser> GetUserCoreInfo(const userver::server::http::HttpRequest& request);


}