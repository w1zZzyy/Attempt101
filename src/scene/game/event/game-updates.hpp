#pragma once

#include "core/logic/move.hpp"
#include "scene/model/event.hpp"

namespace Scene::Game::Event 
{

struct GameUpdateAttempted : public Model::IEvent {
    GameUpdateAttempted(Core::Logic::Move move) noexcept : move(move) {}
    Core::Logic::Move move;
};

struct GameUpdated : public Model::IEvent 
{
    using Sqr = Core::Logic::Square;
    using Promotion = Core::Logic::Piece;
    using RookCastle = std::pair<Sqr, Sqr>;
    using Passant = Sqr;
    using ExtraFlag = std::optional<std::variant<Promotion, Passant, RookCastle>>;

    GameUpdated(
        Sqr from, Sqr targ, 
        ExtraFlag flag = std::nullopt
    ) noexcept : from(from), targ(targ), flag(flag) {}

    Sqr from, targ;
    ExtraFlag flag;
    
};

}