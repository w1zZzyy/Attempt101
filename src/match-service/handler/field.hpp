#pragma once

#include <string_view>

namespace match::handler
{


enum class RequestField
{
    Type,
    DoMove,
    DrawOffer, DrawAccept, DrawDecline,
    Resign,
    Error
};


enum class ResponseField
{
    Type,
    GameUpdated,
    GameResult,
    Message,
    Error
};


constexpr std::string_view FieldName(RequestField type);
constexpr std::string_view FieldName(ResponseField type);


}