#pragma once

#include "boost/json.hpp"

#include "game-logic/handler/game.hpp"
#include "field.hpp"

#include <string_view>
#include <memory>

namespace match::handler
{


class IResponse {
public:
    virtual ResponseField type() const noexcept = 0;
    const boost::json::object& get() const noexcept {return resp;}
protected:
    boost::json::object resp;
};

class GameAffectedResponse : public IResponse {
public:
    GameAffectedResponse(game::handler::GamePtr _g) : game(_g) {}
protected:
    game::handler::GamePtr game;
};

class UpdateResponse : public GameAffectedResponse {
public:
    UpdateResponse(game::handler::GamePtr _g, game::logic::Move move);
    ResponseField type() const noexcept override {
        return ResponseField::GameUpdated;
    };
};

class GameResultResponse: public GameAffectedResponse {
public:
    GameResultResponse(game::handler::GamePtr _g);
    ResponseField type() const noexcept override {
        return ResponseField::GameUpdated;
    };
};

class ErrorResponse : public IResponse {
public:
    ErrorResponse(std::string_view ec);
    ResponseField type() const noexcept override {
        return ResponseField::Error;
    };
};


using ResponsePtr = std::shared_ptr<IResponse>;





}