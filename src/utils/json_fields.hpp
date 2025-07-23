#pragma once

#include <string_view>

namespace game::handler::field 
{

namespace name
{

constexpr std::string_view kType        =   "type";
constexpr std::string_view kNotation    =   "notation";
constexpr std::string_view kResult      =   "result";
constexpr std::string_view kError       =   "error_message";

}

namespace value
{

constexpr std::string_view kTypeMoveDo  =   "do_move";
constexpr std::string_view kTypeMoves   =   "possible_moves";
constexpr std::string_view kTypeFinish  =   "game_finished";
constexpr std::string_view kTypeError   =   "error";
constexpr std::string_view kResultWhite =   "1-0";
constexpr std::string_view kResultBlack =   "0-1";
constexpr std::string_view kResultDraw  =   "1/2-1/2";


}

}