#include "response.hpp"

match::handler::UpdateResponse::UpdateResponse(
    game::handler::GamePtr _g,
    game::logic::Move move) : GameAffectedResponse(_g)
{ 
    resp[FieldName(ResponseField::Type)] = FieldName(ResponseField::GameUpdated);
    game->getMoveUpdates(resp, move); 
}

match::handler::ErrorResponse::ErrorResponse(std::string_view ec)
{
    resp = {
        {FieldName(ResponseField::Type), FieldName(ResponseField::Error)},
        {FieldName(ResponseField::Message), ec}
    };
}

match::handler::GameResultResponse::GameResultResponse(
    game::handler::GamePtr _g) : GameAffectedResponse(_g)
{
    using namespace game::handler;

    std::string_view result = FieldName(ResponseField::GameResult);
    resp[FieldName(ResponseField::Type)] = result;

    switch (game->getResult())
    {
    case GameResult::White:
        resp[result] = "1-0";
        break;
    case GameResult::Black:
        resp[result] = "0-1";
        break;
    case GameResult::Draw:
        resp[result] = "1/2-1/2";
        break;
    case GameResult::InProgress:
        resp[result] = "in_progress";
        break;
    case GameResult::Finished:
        resp[result] = "finished";
        break;
    default:
        break;
    }
}
