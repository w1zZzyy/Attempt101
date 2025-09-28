#pragma once

#include "scene/model/event.hpp"
#include "core/logic/position.hpp"

namespace Scene::Game::Event 
{

class GameStarted : public Model::IEvent {
public:
    GameStarted(const Core::Logic::PositionDM& pos) noexcept :
        pos(pos)
    {}
    const Core::Logic::PositionDM& pos;
};

}