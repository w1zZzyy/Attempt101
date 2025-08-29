#include "view.hpp"

Application::Application()
{
    resource::ConfigManager config;

    Window.create(
        sf::VideoMode(config.WindowSize()), 
        config.Title()
    );
    Window.setFramerateLimit(config.FPS());

    SceneController.Load(config);
}

void Application::run()
{
    SceneController.Handle(Window);
}