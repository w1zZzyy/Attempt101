#include "view.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

#include "../window/view.hpp"

using json = nlohmann::json;

namespace desktop::ui
{
    

desktop::ui::GameScene::GameScene(game::logic::Color side_pov) : 
    side_pov(side_pov)
{
    InitBoard();
}

void GameScene::Render(sf::RenderWindow &window)
{
    board.Render(window, side_pov);
}

void GameScene::SetupWindow(WindowRenderer &window)
{
    std::ifstream file(ASSETS_PATH"/configs/window_config.json");
    
    json config;
    file >> config;

    window
        .setWindowSize(config["width"], config["height"])
        .setTitle(config["title"])
        .setFPS(config["fps"]);
}

void GameScene::InitBoard()
{
    std::ifstream file(ASSETS_PATH"/configs/board_config.json");
    
    json config;
    file >> config;

    const json& wsq = config["square_color"]["white"];
    const json& bsq = config["square_color"]["black"];
    const json& square_size = config["square_size"];
    const json& start = config["start_pos"];

    using namespace game::logic;

    board
        .setSquareColor(WHITE, wsq["red"], wsq["green"], wsq["blue"], wsq["alpha"])
        .setSquareColor(BLACK, bsq["red"], bsq["green"], bsq["blue"], bsq["alpha"])
        .setSquareSize(square_size["width"], square_size["height"])
        .setStartPos(start["x"], start["y"]);
}

void SceneManager::NextScene(WindowRenderer &window)
{
    if(!scene) {
        scene = std::make_unique<GameScene>(game::logic::WHITE);
        scene->SetupWindow(window);
    }
}


}