#pragma once

#include "../idle.hpp"

namespace scene
{


class SceneManager 
{
public:

    void Display(sf::RenderWindow& window) const {scene->Display(window);}
    void Load(const resource::ConfigManager& config);

private:

    ScenePtr scene;

};


}