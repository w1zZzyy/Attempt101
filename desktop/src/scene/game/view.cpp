#include "view.hpp"

namespace scene
{
GameScene::GameScene() : 
    GameController(bus),
    UIController(bus),
    InputController(bus), 
    HumanController(bus),
    AIController(bus)
{}

void GameScene::Display(sf::RenderWindow &window)
{
    window.clear();
    UIController.Render(window);
    window.display();
}

void GameScene::ParseConfig(const resource::ConfigManager &config)
{
    using namespace game::logic;

    const game::logic::Color side = config.BoardView();
    const std::string fen = config.InitalFen();

    UIController
        .SetBoardView(side)
        .SetLeftBottomSquarePos(config.LeftBottomSquare())
        .SetCellColor(WHITE, config.WhiteSquare())
        .SetCellColor(BLACK, config.BlackSquare())
        .SetCellShape(config.SquareSize())
        .SetHighlightedCellColor(config.Highlight());

    GameController.Init(fen);
    HumanController.SetSide(side);

    AIController
        .SetSide(side.opp())
        .SetSearchDepth(config.EngineSearchDepth())
        .LaunchSearchWorker();

    GameController.Update();
}

void GameScene::HandleEvents(sf::RenderWindow &window)
{
    InputController.HandleEvents(window);
}

}