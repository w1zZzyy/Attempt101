#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include "resources/config_manager/view.hpp"
#include "controllers/input/view.hpp"

namespace scene
{


class IScene {
public:
    IScene(event::Bus& bus) : InputController(bus) {}
    virtual ~IScene() = default;
    virtual void Display(sf::RenderWindow& window) = 0;
    virtual void ParseConfig(const resource::ConfigManager& config) = 0;
    void PublishEvents(sf::RenderWindow& window) {InputController.HandleEvents(window);}
protected:
    controller::InputController InputController;
};

using ScenePtr = std::unique_ptr<IScene>;

template<typename T>
concept SceneType = std::is_base_of_v<IScene, T>;

template<SceneType T>
ScenePtr CreateScene() {
    return std::make_unique<T>();
}

}