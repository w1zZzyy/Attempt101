#include "view.hpp"

#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include <userver/crypto/hash.hpp>

#include "../../models/user.hpp"

namespace auth
{

namespace
{


class LoginUser final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-auth-login";

    LoginUser(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& component_context)
        : HttpHandlerBase(config, component_context),
            pg_cluster_(
                component_context
                    .FindComponent<userver::components::Postgres>("chess_db")
                    .GetCluster()) {}

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext&
    ) const override {
        auto user = user::GetUserCoreInfo(request);
        if(!user) 
            return {};

        auto userResult = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "UPDATE chess.users "
            "SET status = $3 "
            "WHERE name = $1 AND password = $2 "
            "RETURNING id",
            user->name, user->password, "online"
        );

        if (userResult.IsEmpty()) {
            auto& response = request.GetHttpResponse();
            response.SetStatus(userver::server::http::HttpStatus::kNotFound);
            return {};
        }

        userver::formats::json::ValueBuilder response;
        response["id"] = userResult.AsSingleRow<std::string>();

        return userver::formats::json::ToString(response.ExtractValue());
    }

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};



}


void AppendLogin(userver::components::ComponentList &component_list)
{
    component_list.Append<LoginUser>();
}


}