#pragma once

#include "scene/view.hpp"

class Application
{
public:
    
    Application();
    void run();

private:

    ui::SceneManager scene;
    sf::RenderWindow window;

};