#include "view.hpp"

#include "response/view.hpp"
#include "json_fields.hpp"
#include <format>

namespace game::handler 
{

using namespace logic;

namespace 
{


std::optional<std::string_view> FindItem(const boost::json::object& req, std::string_view name)
{
    auto iter = req.find(name);
    if(iter == req.end()) {
        return std::nullopt;
    }

    auto& v = iter->value();

    if(v.is_string()) 
    {
        auto& str = v.as_string();
        return std::string_view(
            str.data(),
            str.size()
        );
    }

    return std::nullopt;
}


void SendError(model::MatchPtr match_ptr, std::string_view error_message, Color side)
{
    ErrorResponsePtr resp = ResponseCreator::HandleError(match_ptr);
    resp->handle(error_message, side);
}


}


void DoMoveRequest::handle(const boost::json::object& req, Color side)
{
    if(!match_ptr->is_side_turn(side)) 
        return SendError(match_ptr, "not ur turn to move", side);

    std::optional<std::string_view> notation = FindItem(req, field::name::kType);
    if(!notation)
        return SendError(match_ptr, std::format("field <{}> is empty", field::name::kNotation), side);

    auto move = match_ptr->do_move(*notation);
    if(move) {
        MoveResponsePtr resp = ResponseCreator::HandleMove(match_ptr);
        return resp->handle(*move, side);
    }

    return SendError(match_ptr, move.error(), side);
}


void HandleRequest(model::MatchPtr mp, Color side, const boost::json::object& req)
{   
    std::optional<std::string_view> type = FindItem(req, field::name::kType);

    if(!type) 
        return SendError(mp, std::format("field <{}> is empty", field::name::kType), side);

    RequestPtr req_ptr;

    if(*type == field::value::kTypeMoveDo)
        req_ptr = std::make_unique<DoMoveRequest>(mp);

    if(req_ptr) 
        return req_ptr->handle(req, side);

    return SendError(mp, "unrecognized request type", side);
}


}