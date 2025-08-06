#include "view.hpp"

#include "resources/textures_manager/view.hpp"
#include "../models/square.hpp"

namespace ui
{

void GameScene::Render(sf::RenderWindow &window)
{
    board.Render(window);
    for(auto& entity : entities) 
        entity->Render(window);
}

void GameScene::ReadConfig(const resource::BoardConfigManager &config)
{
    using namespace game::logic;

    const sf::Vector2f square_size = config.SquareSize();

    model::Square::Init(player_side, config.LeftBottomSquare(), square_size);

    board 
        .setSquareColor(WHITE, config.WhiteSquare())
        .setSquareColor(BLACK, config.BlackSquare())
        .setSquareSize(square_size);

    game_logic.Init(config.InitalFen());

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(auto piece = game_logic.getPieceOn(sqr); piece.isValid()) 
        {
            auto clr = game_logic.getPieceClr(sqr);
            auto& entity = entities.emplace_back(model::CreateEntity<PieceEntity>(
                *resource::TextureManager::Get(clr, piece)
            ));
            entity->setPos(model::Square::GetPos(sqr)).setSize(square_size);
        }
    }
}

SceneManager::SceneManager()
{
    resource::BoardConfigManager config;
    scene = std::make_unique<GameScene>(game::logic::WHITE);
    scene->ReadConfig(config);
}


}