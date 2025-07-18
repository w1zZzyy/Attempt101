#include "view.hpp"

#include "json_fields.hpp"

using namespace game::logic;

namespace game::handler 
{


void IMoveResponse::do_move(Move move)
{
    boost::json::object update;
    
    update[field::name::kType]      =   field::value::kTypeMoveDo;
    update[field::name::kNotation]  =   move.to_string();

    match_ptr->update(update, WHITE, BLACK);
}


void DoMoveResponse::handle(Move move, Color side)
{
    do_move(move);

    boost::json::array update_moves;
    const DefaultMoveList& moves = match_ptr->get_moves();
    
    for(size_t i = 0; i < moves.get_size(); ++i) 
        update_moves.emplace_back(moves[i].to_string());

    boost::json::object update;
    update[field::name::kType]      =   field::value::kTypeMoves;
    update[field::name::kNotation]  =   update_moves;

    match_ptr->update(update, side);
}


void FatalMoveResponse::handle(Move move, Color side)
{
    do_move(move);

    boost::json::object update;
    update[field::name::kType] = field::value::kTypeFinish;

    switch (match_ptr->get_result()) {
        case model::MatchResult::White: update[field::name::kResult] = field::value::kResultWhite;  break;
        case model::MatchResult::Black: update[field::name::kResult] = field::value::kResultBlack;  break;
        case model::MatchResult::Draw:  update[field::name::kResult] = field::value::kResultDraw;   break;
        default: break;
    }

    match_ptr->update(update, WHITE, BLACK);
}


void ErrorResponse::handle(std::string_view error_message, Color side)
{
    boost::json::object error;

    error[field::name::kType]   =   field::value::kTypeError;
    error[field::name::kError]  =   error_message;

    match_ptr->update(error, side);
}



MoveResponsePtr ResponseCreator::HandleMove(model::MatchPtr mp)
{
    model::MatchResult res = mp->get_result();
    if(res != model::MatchResult::Continues) {
        return std::make_unique<FatalMoveResponse>(mp);
    }
    return std::make_unique<DoMoveResponse>(mp);
}


ErrorResponsePtr ResponseCreator::HandleError(model::MatchPtr mp)
{
    return std::make_unique<ErrorResponse>(mp);
}


}