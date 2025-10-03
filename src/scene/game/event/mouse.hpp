#pragma once

#include "core/logic/square.hpp"
#include "scene/shared/event/mouse.hpp"

#include <optional>

namespace Scene::Game::Event
{

struct OnBoard : Shared::Event::Mouse {
    std::optional<Core::Logic::Square> sqr;
};

struct MousePressed : OnBoard {};
struct MouseReleased : OnBoard {};
struct MouseMoved : OnBoard {};

}