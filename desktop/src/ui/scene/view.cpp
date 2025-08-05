#include "view.hpp"

#include "resources/textures_manager/view.hpp"

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

    SquarePosition::Init(player_side, config.LeftBottomSquare(), square_size);

    board 
        .setSquareColor(WHITE, config.WhiteSquare())
        .setSquareColor(BLACK, config.BlackSquare())
        .setSquareSize(square_size);

    pos.set_fen(config.InitalFen());
    update_game();

    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        if(auto piece = pos.piece_on(sqr); piece.isValid()) 
        {
            auto clr = pos.piece_clr_on(sqr);
            auto entity = entities.emplace_back(std::make_shared<PieceEntity>(
                *resource::TextureManager::Get(clr, piece)
            ));
            entity->setPos(SquarePosition::GetPos(sqr)).setSize(square_size);
        }
    }
}

void GameScene::update_game()
{
    using namespace game::logic;

    pos.compute_enemy_attackers().compute_pins_from_sliders();
    legal_moves.generate(pos);

    if(pos.is_check()) {
        status = legal_moves.empty() 
        ?  static_cast<GameStatus>(int(pos.get_side().opp()))
        : GameStatus::Draw;
    } else if(pos.is_draw()) {
        status = GameStatus::Draw;
    } else {
        status = GameStatus::InProgress;
    }
}

SceneManager::SceneManager()
{
    resource::BoardConfigManager config;
    scene = std::make_unique<GameScene>(game::logic::WHITE);
    scene->ReadConfig(config);
}


}