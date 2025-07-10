#include "field.hpp"

constexpr std::string_view match::handler::FieldName(RequestField type)
{
    switch (type)
    {
    case RequestField::Type:            return "type";
    case RequestField::DoMove:          return "do_move";
    case RequestField::DrawOffer:       return "draw_offer";
    case RequestField::DrawAccept:      return "draw_accept";
    case RequestField::DrawDecline:     return "draw_decline";  
    case RequestField::Error:           return "error";
    default:                            break;
    }
    return "unrecognized";
}

constexpr std::string_view match::handler::FieldName(ResponseField type)
{
    switch (type)
    {
    case ResponseField::Type:           return "type";
    case ResponseField::GameUpdated:    return "game_updated";   
    case ResponseField::GameResult:     return "game_result";
    case ResponseField::Message:        return "message";
    case ResponseField::Error:          return "error";
    default:                            break;
    }
    return "unrecognized";
}
