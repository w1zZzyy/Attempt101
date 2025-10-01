#pragma once

#include "SFML/System/Vector2.hpp"
#include "scene/model/event.hpp"

namespace Scene::Shared::Event
{

struct Mouse : public Model::IEvent {
    Mouse(sf::Vector2f pos) noexcept : pos(pos) {}
    sf::Vector2f pos;
};

struct MousePressed : Mouse {};
struct MouseReleased : Mouse {};
struct MouseMoved : Mouse {};

}