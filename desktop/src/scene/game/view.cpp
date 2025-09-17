#include "view.hpp"
#include "controllers/game/view.hpp"

namespace scene
{
GameScene::GameScene() : 
    GameController(bus),
    UIController(bus),
    AIController(bus), 
    IScene(bus) 
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
        .SetOrigin(config.LeftBottomSquare())
        .SetCellColor(WHITE, config.WhiteSquare())
        .SetCellColor(BLACK, config.BlackSquare())
        .SetCellShape(config.SquareSize())
        .SetHighlightedCellColor(config.Highlight())
        .SetOnPieceSelected(
            [this](game::logic::Square sqr) { 
                const controller::GameLogic& logic = GameController.GetLogic();
                return logic.MovesFrom(sqr);
            }
        );

    GameController.Init(fen);

    AIController
        .SetSide(side.opp())
        .SetPosition(GameController.GetLogic().GetPosition())
        .SetSearchDepth(config.EngineSearchDepth())
        .LaunchSearchWorker();
}

}