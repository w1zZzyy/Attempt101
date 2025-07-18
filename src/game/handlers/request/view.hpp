#pragma once

#include "models/match/view.hpp"

namespace game::handler
{


class IRequest {
public:
    IRequest(model::MatchPtr mp) : match_ptr(mp) {}
    virtual ~IRequest() = default;
    virtual void handle(const boost::json::object& req, logic::Color requester_side) = 0;
protected:
    model::MatchPtr match_ptr;
};
using RequestPtr = std::unique_ptr<IRequest>;


class DoMoveRequest : public IRequest {
public:
    DoMoveRequest(model::MatchPtr mp) : IRequest(mp) {}
    void handle(const boost::json::object& req, logic::Color requester_side) override;
};


void HandleRequest(model::MatchPtr match_ptr, logic::Color requester_side, const boost::json::object& req);


}