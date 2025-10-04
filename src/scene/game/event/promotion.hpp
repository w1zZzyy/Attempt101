#pragma once 

#include "logic/square.hpp"
#include "scene/model/event.hpp"

namespace Scene::Game::Event 
{

struct Promotion : Model::IEvent {
    Promotion(Core::Logic::Square from, Core::Logic::Square on) noexcept : 
        from(from), on(on) 
    {}
    Core::Logic::Square from, on;
};

}