#pragma once

/// @file userver/storages/query.hpp
/// @brief @copybrief storages::Query

#include <optional>
#include <string>

#include <userver/utils/string_literal.hpp>
#include <userver/utils/strong_typedef.hpp>

USERVER_NAMESPACE_BEGIN

namespace tracing {
class Span;
}  // namespace tracing

namespace storages {

/// @brief Holds a query, its name and logging mode.
///
/// Prefer using via @ref scripts/docs/en/userver/sql_files.md or use a const variable with `name` specified.
///
/// @note You may write a query in `.sql` file and generate a header file with Query from it.
///       See @ref scripts/docs/en/userver/sql_files.md for more information.
class Query {
public:
    /// String with query name
    using Name = utils::StrongTypedef<struct NameTag, std::string>;

    /// Zero terminated view to the query name
    using NameView = utils::zstring_view;

    /// Compile time literal with query name
    class NameLiteral : public utils::zstring_view {
    public:
        NameLiteral() = delete;

#if defined(__clang__) && __clang_major__ < 18
        // clang-16 and below lose (optimize out) the pointer to `literal` with consteval. Clang-18 is know to work
        constexpr
#else
        USERVER_IMPL_CONSTEVAL
#endif
            NameLiteral(const char* literal) noexcept
            : zstring_view{literal} {
        }
    };

    enum class LogMode : unsigned char { kFull, kNameOnly };

    Query() = default;
    ~Query() = default;

    Query(const Query& other) = default;
    Query(Query&& other) = default;
    Query& operator=(const Query& other) = default;
    Query& operator=(Query&& other) = default;

    /*TODO: constexpr*/ Query(utils::StringLiteral statement, NameLiteral name, LogMode log_mode = LogMode::kFull)
        : Query(std::string{statement}, Name{std::string{name}}, log_mode) {}

    Query(const char* statement, std::optional<Name> name = std::nullopt, LogMode log_mode = LogMode::kFull)
        : Query(std::string{statement}, std::move(name), log_mode) {}
    Query(std::string statement, std::optional<Name> name = std::nullopt, LogMode log_mode = LogMode::kFull)
        : dynamic_{statement, std::move(name)}, log_mode_(log_mode) {}

    std::optional<NameView> GetNameView() const noexcept;
    const std::optional<Name>& GetName() const { return dynamic_.name_; }

    utils::zstring_view GetStatementView() const noexcept;
    const std::string& Statement() const { return dynamic_.statement_; }

    /// @brief Fills provided span with connection info
    void FillSpanTags(tracing::Span&) const;

private:
    struct DynamicStrings {
        std::string statement_{};
        std::optional<Name> name_{};
    };

    DynamicStrings dynamic_{};

    LogMode log_mode_ = LogMode::kFull;
};

}  // namespace storages

USERVER_NAMESPACE_END
