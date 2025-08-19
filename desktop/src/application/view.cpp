#include "view.hpp"

#include "logic/position.hpp"

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
    while(Window.isOpen())
    {
        SceneController.HandleEvents(Window);
        SceneController.Display(Window);
    }
}