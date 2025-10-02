#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "scene/shared/bus.hpp"

namespace Scene::Model 
{

template<typename T>
class IScene {
public:

    IScene(sf::RenderWindow& window, Shared::Bus& bus) noexcept : 
        window(window), 
        bus(bus) 
    {}

    void Render() {
        window.clear();
        cast()->RenderImpl();
        window.display();
    }

protected:

    sf::RenderWindow& window;
    Shared::Bus& bus;

private:

    T* cast() noexcept {return static_cast<T*>(this);}

};

template<typename T>
concept SceneType = std::derived_from<T, IScene<T>> && requires {
    typename T::Builder;
};

}