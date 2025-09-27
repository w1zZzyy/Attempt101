#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "core/logic/defs.hpp"
#include "ui/renderer/board.hpp"
#include <optional>

int main()
{
    sf::RenderWindow Window(sf::VideoMode({600, 600}), "Final Attempt");
    Window.setVerticalSyncEnabled(true);

    UI::Renderer::Board Board;
    UI::Renderer::Board::Options opt;

    opt.size = {500, 500};
    opt.origin = {50, 500};
    opt.padding = 15;
    opt.player = Core::Logic::BLACK;

    Board.Init(opt);

    while(Window.isOpen())
    {
        while(std::optional event = Window.pollEvent())
        {
            if(event->getIf<sf::Event::Closed>()) {
                Window.close();
            }
        }

        Window.clear();
        Board.Render(Window);
        Window.display();
    }
}