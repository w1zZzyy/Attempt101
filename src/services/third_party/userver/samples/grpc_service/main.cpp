#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/daemon_run.hpp>

#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/client/component_list.hpp>
#include <userver/ugrpc/server/component_list.hpp>

#include <call_greeter_client_test_handler.hpp>
#include <greeter_client.hpp>
#include <greeter_service.hpp>

/// [main]
int main(int argc, char* argv[]) {
    const auto component_list =  //
        components::MinimalServerComponentList()
            .Append<components::TestsuiteSupport>()
            .Append<congestion_control::Component>()
            // Default client factory and middlewares. You can create multiple instances of
            // client factory component using `.Append<T>("name")` if different gRPC clients
            // require different credentials or different grpc-core options.
            .Append<ugrpc::client::ClientFactoryComponent>()
            .AppendComponentList(ugrpc::client::MinimalComponentList())
            // All gRPC services are registered in this component.
            .AppendComponentList(ugrpc::server::MinimalComponentList())
            // Custom components:
            .Append<samples::GreeterClientComponent>()
            .Append<samples::GreeterServiceComponent>()
            .Append<samples::CallGreeterClientTestHandler>();
    return utils::DaemonMain(argc, argv, component_list);
}
/// [main]
