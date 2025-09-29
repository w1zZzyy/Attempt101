#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/position.hpp"
#include "scene/game/event/game-started.hpp"
#include "ui/model/options.hpp"
#include "ui/renderer/pieces.hpp"
#include "ui/renderer/board.hpp"
#include "scene/game/state/machine.hpp"

#include <optional>

int main()
{
    const sf::Vector2u window_size = {600, 600};

    sf::RenderWindow Window(sf::VideoMode(window_size), "Final Attempt");
    Window.setVerticalSyncEnabled(true);

    UI::Options::BoardVisual BoardV;
    auto BoardOpt = UI::Options::Board::Builder()
        .setPlayer(Core::Logic::WHITE)
        .setWindowSize(window_size)
        .build();
    BoardV.Init(BoardOpt);

    UI::Renderer::Board Board(BoardV);
    UI::Renderer::Pieces Pieces(BoardV);
    Scene::Game::State::PieceMachine Machine(Pieces);
    Core::Logic::PositionDM pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Board.Init(BoardOpt);

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