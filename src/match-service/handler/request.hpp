#pragma once

#include "player.hpp"
#include "response.hpp"

namespace match::handler
{


class IRequest {
public:
    IRequest() {}
    ~IRequest() {}
    virtual ResponsePtr response(const boost::json::object& req) = 0;
};

class IPlayerRequest : public IRequest {
public:
    IPlayerRequest(model::PlayerPtr _p) : player(_p) {}
protected:
    model::PlayerPtr player;
};

class MoveRequest : public IPlayerRequest {
public:
    MoveRequest(model::PlayerPtr _p) : IPlayerRequest(_p) {}
    ResponsePtr response(const boost::json::object& req) override;
};

class ErrorRequest : public IRequest {
public:
    ErrorRequest(std::string_view ec) : ec(ec) {}
    ResponsePtr response(const boost::json::object& req) override;
protected:
    std::string_view ec;
};


class PlayerRequestFactory {
    using PlayerRequestPtr = std::shared_ptr<IPlayerRequest>;
public:
    static PlayerRequestPtr Create(
        std::string_view type, model::PlayerPtr _p
    );
};


struct RequestManager
{
    RequestManager(model::PlayerPtr _p, const boost::json::object& req);
    std::shared_ptr<IRequest> handler;
};


}