#pragma once

#include "scene/manager/view.hpp"

class Application {
public:

    Application();
    void run();
    
private:

    scene::SceneManager SceneController;
    sf::RenderWindow Window;

};