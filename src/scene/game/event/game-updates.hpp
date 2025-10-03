#pragma once

#include "core/logic/move.hpp"
#include "scene/model/event.hpp"

namespace Scene::Game::Event 
{

struct GameUpdateAttempt : public Model::IEvent {
    GameUpdateAttempt(Core::Logic::Move move) noexcept : move(move) {}
    Core::Logic::Move move;
};

}