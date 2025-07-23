#pragma once

#include <grpcpp/impl/service_type.h>

#include <userver/ugrpc/impl/static_service_metadata.hpp>

USERVER_NAMESPACE_BEGIN

namespace ugrpc::server::impl {

/// @brief Listens to requests for a gRPC service, forwarding them to a
/// user-provided service implementation. ServiceWorker instances are
/// created and owned by `Server`; services, on the other hand, are created
/// and owned by the user.
/// @note Must be destroyed after the corresponding `CompletionQueue`
class ServiceWorker {
public:
    ServiceWorker& operator=(ServiceWorker&&) = delete;
    virtual ~ServiceWorker();

    /// Get the grpcpp service for registration in the `ServerBuilder`
    virtual grpc::Service& GetService() = 0;

    /// Get the static per-gRPC-service metadata provided by codegen
    virtual const ugrpc::impl::StaticServiceMetadata& GetMetadata() const = 0;

    /// Start serving requests. Should be called after the grpcpp server starts.
    virtual void Start() = 0;
};

}  // namespace ugrpc::server::impl

USERVER_NAMESPACE_END
