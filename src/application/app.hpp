#pragma once

#include "scene/game/scene.hpp"
#include "scene/shared/bus.hpp"
#include "scene/shared/input.hpp"

class App 
{
public:

    App();
    void run(int argc, char* argv[]);

private:

    Scene::GameScene::Builder Make(int argc, char* argv[]);

private:

    Scene::Shared::Bus bus;
    Scene::Shared::Input input;
    sf::RenderWindow window;

};