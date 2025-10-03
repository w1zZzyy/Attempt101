#pragma once 

#include "scene/shared/bus.hpp"
#include "core/engine/search.hpp"
#include "core/logic/position.hpp"

namespace Scene::Game::Handler 
{

class AIHandler 
{
public:

    AIHandler(Shared::Bus& bus) noexcept : bus(bus) {}
    
    void Init(const Core::Engine::Search::Options&);
    void Start() {engine.Launch();}

private:

    template<Model::EventType TEvent>
    void OnGame();

private:

    Shared::Bus& bus;
    Core::Engine::Search engine;
    const Core::Logic::PositionDM* pos;
    Core::Logic::Color player;

};

}