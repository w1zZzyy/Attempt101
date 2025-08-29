#pragma once

#include "idle.hpp"

namespace scene
{


class SceneManager 
{
public:

    void Handle(sf::RenderWindow& window);
    void Load(const resource::ConfigManager& config);

private:

    ScenePtr scene;

};


}