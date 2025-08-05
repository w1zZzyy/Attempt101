#include "view.hpp"

Application::Application()
{
    resource::WindowConfigManager config;
    window.create(
        sf::VideoMode(config.WindowSize()), 
        config.Title()
    );
    window.setFramerateLimit(config.FPS());
}

void Application::run()
{
    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        scene.Render(window);
        window.display();
    }
}