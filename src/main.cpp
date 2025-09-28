#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/position.hpp"
#include "scene/game/event/game-started.hpp"
#include "ui/renderer/pieces.hpp"
#include "ui/renderer/board.hpp"
#include "scene/game/state/machine.hpp"

#include <optional>

int main()
{
    sf::RenderWindow Window(sf::VideoMode({600, 600}), "Final Attempt");
    Window.setVerticalSyncEnabled(true);

    UI::Renderer::Board Board;
    UI::Renderer::Pieces Pieces;
    Scene::Game::State::PieceMachine Machine(Pieces);
    Core::Logic::PositionDM pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    UI::Model::Options opt;

    opt.size = {500, 500};
    opt.origin = {50, 500};
    opt.padding = 20;
    opt.player = Core::Logic::WHITE;

    Board.Init(opt);
    Pieces.Init(opt);

    Machine.HandleEvent(Scene::Game::Event::GameStarted{pos});

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
        Pieces.Render(Window);
        Window.display();
    }
}