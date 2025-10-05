#include "app.hpp"

#include "parser.hpp"

namespace 
{

Core::Logic::Color RandomSide()
{
    srand(time(NULL));
    return {rand() % 2};
}

}

App::App() : 
    input(bus), 
    window(sf::VideoMode::getDesktopMode(), "Final Attempt", sf::Style::Titlebar | sf::Style::Close)
{
    window.setPosition({0,0});
    window.setVerticalSyncEnabled(true);
    bus.SwitchMode(Scene::Shared::Bus::Mode::AddingListeners);
}

void App::run(int argc, char* argv[])
{
    Scene::GameScene scene = Make(argc, argv).build();
    bus.Launch();

    while(window.isOpen())
    {
        input.handleEvents(window);
        scene.Render();
    }
}

Scene::GameScene::Builder App::Make(int argc, char *argv[])
{
    Parser parser(argc, argv);

    Core::Logic::Color player = parser.player().value_or(RandomSide());
    std::string fen = parser.fen().value_or("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    UI::Options::Board board = 
        UI::Options::Board::Builder()
        .setPlayer(player)
        .setWindowSize(window.getSize())
        .build();

    Core::Engine::Search::Options engine;
    engine.maxDepth = parser.max_depth().value_or(Core::Logic::MAX_HISTORY_SIZE - 1);
    engine.timeSec = parser.time_limit().value_or(3);
    engine.ttSizeMB = parser.tt_size().value_or(64);

    return Scene::GameScene::Builder()
            .setBoardView(board)
            .setEngineOptions(engine)
            .setFen(fen)
            .setPlayer(player)
            .setWindow(window)
            .setEventBus(bus);
}
