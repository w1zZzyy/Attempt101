#pragma once

#include "../idle.hpp"

namespace scene
{


class SceneManager 
{
public:

    void Display(sf::RenderWindow& window) {scene->Display(window);}
    void HandleEvents(sf::RenderWindow& window) {scene->HandleEvents(window);}
    void Load(const resource::ConfigManager& config);

private:

    ScenePtr scene;

};


}