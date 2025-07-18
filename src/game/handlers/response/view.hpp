#pragma once

#include <boost/json.hpp>
#include "models/match/view.hpp"

namespace game::handler
{


class IResponse {
public:
    IResponse(model::MatchPtr mp) : match_ptr(mp) {}
    virtual ~IResponse() = default;
protected:
    model::MatchPtr match_ptr;
};


class IMoveResponse : public IResponse {
public:
    IMoveResponse(model::MatchPtr mp) : IResponse(mp) {}
    virtual ~IMoveResponse() = default;
    virtual void handle(logic::Move move, logic::Color requester_side) = 0;
protected:
    void do_move(logic::Move move);
};


class DoMoveResponse : public IMoveResponse {
public:
    DoMoveResponse(model::MatchPtr mp) : IMoveResponse(mp) {}
    void handle(logic::Move move, logic::Color requester_side) override;
};


class FatalMoveResponse : public IMoveResponse {
public:
    FatalMoveResponse(model::MatchPtr mp) : IMoveResponse(mp) {}
    void handle(logic::Move move, logic::Color requester_side) override;
};


class ErrorResponse : public IResponse {
public:
    ErrorResponse(model::MatchPtr mp) : IResponse(mp) {}
    void handle(std::string_view error_message, logic::Color requester_side);
};


using MoveResponsePtr   =   std::unique_ptr<IMoveResponse>;
using ErrorResponsePtr  =   std::unique_ptr<ErrorResponse>;

class ResponseCreator
{
public:
    static MoveResponsePtr HandleMove(model::MatchPtr match_ptr);
    static ErrorResponsePtr HandleError(model::MatchPtr match_ptr);
};


}