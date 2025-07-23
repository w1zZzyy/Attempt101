#include "user.hpp"

#include <userver/crypto/hash.hpp>

namespace user
{


constexpr size_t PASSWORD_MIN_LEN = 8;
constexpr size_t NAME_MAX_LEN     = 32;


std::optional<TUser> GetUserCoreInfo(const userver::server::http::HttpRequest& request)
{
    auto name = request.GetFormDataArg("name").value;
    auto password = request.GetFormDataArg("password").value;

    if(name.empty() || name.size() > NAME_MAX_LEN || password.size() < user::PASSWORD_MIN_LEN) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return std::nullopt;
    }

    TUser user;
    user.name = name;
    user.password = userver::crypto::hash::Sha256(password);

    return user;
}


}