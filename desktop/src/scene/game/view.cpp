#include "view.hpp"

namespace scene
{
GameScene::GameScene() : 
    GameController(bus),
    UIController(bus)
{}

void GameScene::Display(sf::RenderWindow &window)
{
    window.clear();
    UIController.Render(window);
    window.display();
}

void GameScene::ParseConfig(const resource::ConfigManager &config)
{
    auto board_config = dynamic_cast<const resource::BoardConfigManager*>(&config);
    assert(board_config != nullptr && "Config is not a BoardConfigManager");

    using namespace game::logic;
    
    UIController
        .SetBoardView(board_config->BoardView())
        .SetLeftBottomSquarePos(board_config->LeftBottomSquare())
        .SetCellColor(WHITE, board_config->WhiteSquare())
        .SetCellColor(BLACK, board_config->BlackSquare())
        .SetCellShape(board_config->SquareSize())
        .SetHighlightedCellColor(board_config->Highlight());

    GameController.Init(board_config->InitalFen());
}

}