#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/clients/dns/component.hpp>    
#include <userver/clients/http/component.hpp>

#include "handlers/login/view.hpp"
#include "handlers/register/view.hpp"

int main(int argc, char* argv[])
{
    auto component_list = 
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::components::HttpClient>()
            .Append<userver::components::Postgres>("chess_db");

    auth::AppendRegister(component_list);
    auth::AppendLogin(component_list);

    return userver::utils::DaemonMain(argc, argv, component_list);
}