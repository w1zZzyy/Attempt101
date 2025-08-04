#include "ui/window/view.hpp"

int main()
{
    desktop::ui::WindowRenderer window;
    
    while(window.IsOpen()) 
    {
        while(auto event = window.PollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.Close();
        }

        window.Draw();
    }
}