#include "view.hpp"

#include "logic/src/position.hpp"

Application::Application()
{
    game::logic::Position::Init();

    resource::WindowConfigManager window_config;
    resource::BoardConfigManager board_config;

    Window.create(
        sf::VideoMode(window_config.WindowSize()), 
        window_config.Title()
    );
    Window.setFramerateLimit(window_config.FPS());

    SceneController.Load(board_config);
}

void Application::run()
{
    while(Window.isOpen())
    {
        SceneController.HandleEvents(Window);
        SceneController.Display(Window);
    }
}