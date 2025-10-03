#pragma once

#include "core/logic/defs.hpp"
#include "core/logic/movelist.hpp"
#include "core/logic/position.hpp"
#include "scene/shared/bus.hpp"

#include <optional>

namespace Scene::Game::Handler 
{

class GameHandler {
public:

    GameHandler(Shared::Bus&);

    void Init(const std::string& fen, Core::Logic::Color player);
    void Start();

private:

    void Update();
    void OnUpdateAttempt();

private:

    Shared::Bus& bus;

    Core::Logic::PositionDM pos;
    Core::Logic::MoveList moves;

    Core::Logic::Color player;
    std::optional<std::string> result;

};

}