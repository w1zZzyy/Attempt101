#pragma once

/// @file userver/middlewares/pipeline.hpp
/// @brief Lists all available middlewares and builds their order of execution.

#include <unordered_map>

#include <userver/components/component_base.hpp>
#include <userver/components/component_fwd.hpp>
#include <userver/utils/meta_light.hpp>
#include <userver/yaml_config/fwd.hpp>

#include <userver/middlewares/impl/middleware_pipeline_config.hpp>

USERVER_NAMESPACE_BEGIN

/// Pipeline to configure middlewares order and static configs.
/// @see @ref scripts/docs/en/userver/grpc/middlewares_order.md
/// @see @ref scripts/docs/en/userver/grpc/middlewares_configuration.md
namespace middlewares {

/// @brief The dependency type between middlewares.
///
/// Iff dependency type from 'X' to 'Y' is `kStrong` and 'Y' is disabled, middlewares pipeline will failure when start,
/// otherwise (`kWeak`) we ignore this dependency.
enum class DependencyType {
    kStrong = 0,
    kWeak = 1,
};

namespace impl {

struct Connect final {
    std::string node_name{};
    DependencyType type{DependencyType::kStrong};
};

struct MiddlewareDependency final {
    std::string middleware_name{};
    std::vector<Connect> befores{};
    std::vector<Connect> afters{};
    bool enabled{true};
    std::optional<std::string> group{};
};

using Dependencies = std::unordered_map<std::string, MiddlewareDependency>;

class MiddlewarePipeline final {
public:
    explicit MiddlewarePipeline(Dependencies&& deps);

    std::vector<std::string> GetPerServiceMiddlewares(const MiddlewareRunnerConfig& config) const;

    const MiddlewareOrderedList& GetOrderedList() const { return pipeline_; }

private:
    Dependencies deps_{};
    MiddlewareOrderedList pipeline_{};
};

template <typename Group>
std::string BeginOfGroup() {
    return std::string{Group::kName} + "#begin";
}

template <typename Group>
std::string EndOfGroup() {
    return std::string{Group::kName} + "#end";
}

/// @brief A base component to create middlewares pipeline.
///
/// ## Static options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// middlewares | middlewares names and configs to use | `{}`
///
/// ## Config example:
///
/// @snippet grpc/functional_tests/middleware_server/static_config.yaml middleware pipeline component config

class AnyMiddlewarePipelineComponent : public components::ComponentBase {
public:
    AnyMiddlewarePipelineComponent(
        const components::ComponentConfig& config,
        const components::ComponentContext& context,
        impl::BuiltInConfig&& builtin_config
    );

    static yaml_config::Schema GetStaticConfigSchema();

    /// @cond
    /// Only for internal use.
    const impl::MiddlewarePipeline& GetPipeline() const;
    /// @endcond

private:
    impl::MiddlewarePipeline pipeline_;
};

}  // namespace impl

/// @brief Specifies a middleware's order with respect to other middlewares.
///
/// If you don't care about the order in relative to others, ignore this and your middleware
/// will be in the `kUser` group.
/// Otherwise, pass an instance of this class to `MiddlewareComponentBase` in the constructor of your
/// middleware component.
class MiddlewareDependencyBuilder final {
public:
    /// @brief Make an empty dependency builder. For middlewares, `InGroup` must be called at some point.
    MiddlewareDependencyBuilder() = default;

    MiddlewareDependencyBuilder(const MiddlewareDependencyBuilder&) = default;
    MiddlewareDependencyBuilder(MiddlewareDependencyBuilder&&) noexcept = default;
    MiddlewareDependencyBuilder& operator=(const MiddlewareDependencyBuilder&) = default;
    MiddlewareDependencyBuilder& operator=(MiddlewareDependencyBuilder&&) = default;

    /// @brief Add dependency for your middleware. Your middleware will be before `MiddlewareBefore` in the pipeline
    /// @param type is connection type between middlewares
    template <typename MiddlewareBefore>
    MiddlewareDependencyBuilder Before(DependencyType type = DependencyType::kStrong) &&;

    /// @brief Add dependency for your middleware. Your middleware will be before `before` in the pipeline
    /// @param type is connection type between middlewares
    /// @param before is the middleware component name
    MiddlewareDependencyBuilder Before(std::string_view before, DependencyType type = DependencyType::kStrong) &&;

    /// @brief Add dependency for your middleware. Your middleware will be after `MiddlewareAfter` in the pipeline
    /// @param type is connection type between middlewares
    template <typename MiddlewareAfter>
    MiddlewareDependencyBuilder After(DependencyType type = DependencyType::kStrong) &&;

    /// @brief Add dependency for your middleware. Your middleware will be after `after` in the pipeline
    /// @param type is connection type between middlewares
    /// @param after is the middleware component name
    MiddlewareDependencyBuilder After(std::string_view after, DependencyType type = DependencyType::kStrong) &&;

    /// @brief Add dependency for your middleware. Your middleware will be in the `Group` group
    template <typename Group>
    MiddlewareDependencyBuilder InGroup() &&;

    /// @cond
    // Only for internal use.
    impl::MiddlewareDependency ExtractDependency(std::string_view middleware_name) &&;
    impl::MiddlewareDependency ExtractGroupDependency(std::string_view group_name) &&;
    /// @endcond

private:
    void InUserGroupByDefault();

    impl::MiddlewareDependency dep_{};
};

namespace impl {

template <typename T>
using IsGroup = decltype(T::kDependency);

template <typename T>
constexpr bool kIsGroup = std::is_same_v<meta::DetectedType<IsGroup, T>, const MiddlewareDependencyBuilder>;

}  // namespace impl

template <typename MiddlewareBefore>
MiddlewareDependencyBuilder MiddlewareDependencyBuilder::Before(DependencyType type) && {
    if constexpr (impl::kIsGroup<MiddlewareBefore>) {
        return std::move(*this).Before(impl::BeginOfGroup<MiddlewareBefore>(), type);
    }
    return std::move(*this).Before(MiddlewareBefore::kName, type);
}

template <typename MiddlewareAfter>
MiddlewareDependencyBuilder MiddlewareDependencyBuilder::After(DependencyType type) && {
    if constexpr (impl::kIsGroup<MiddlewareAfter>) {
        return std::move(*this).After(impl::EndOfGroup<MiddlewareAfter>(), type);
    }
    return std::move(*this).After(MiddlewareAfter::kName, type);
}

template <typename Group>
MiddlewareDependencyBuilder MiddlewareDependencyBuilder::InGroup() && {
    dep_.group = Group::kName;
    dep_.afters.push_back(impl::Connect{impl::BeginOfGroup<Group>(), DependencyType::kStrong});
    return std::move(*this).Before(impl::EndOfGroup<Group>(), DependencyType::kWeak);
}

}  // namespace middlewares

USERVER_NAMESPACE_END
