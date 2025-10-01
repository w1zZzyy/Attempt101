#pragma once 

#include "SFML/Window/Event.hpp"
#include "bus.hpp"
#include "scene/model/event.hpp"

namespace Scene::Shared 
{

class Input {
public:

    Input(Bus& bus) noexcept : bus(bus) {}
    void handleEvent(sf::Event&);

private:

    template<Model::EventType TEvent, typename T>
    bool proceed(sf::Event& event);

private:

    Bus& bus;

};

}