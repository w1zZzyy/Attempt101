#include "view.hpp"

#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

#include "../../models/user.hpp"

namespace auth 
{

namespace {

class RegisterUser final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-auth-register";

    RegisterUser(const userver::components::ComponentConfig& config,
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

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO chess.users(name, password, status) VALUES($1, $2, $3) "
            "ON CONFLICT DO NOTHING "
            "RETURNING users.id",
            user->name, user->password, "online"
        );


        if(result.IsEmpty()) {
            auto& response = request.GetHttpResponse();
            response.SetStatus(userver::server::http::HttpStatus::kConflict);
            return {};
        }

        userver::formats::json::ValueBuilder response;
        response["id"] = result.AsSingleRow<std::string>();

        return userver::formats::json::ToString(response.ExtractValue());
    }
private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  


void AppendRegister(userver::components::ComponentList &component_list)
{
    component_list.Append<RegisterUser>();
}


}