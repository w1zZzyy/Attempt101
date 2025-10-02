#include "scene.hpp"

namespace Scene
{

GameScene::Builder &GameScene::Builder::setWindow(sf::RenderWindow &window) noexcept
{
    this->window = &window;
    return *this;
}

GameScene::Builder &GameScene::Builder::setEventBus(Shared::Bus& bus) noexcept
{
    this->bus = &bus;
    return *this;
}

GameScene::Builder& GameScene::Builder::setFen(std::string fen) noexcept
{
    this->fen = std::move(fen);
    return *this;
}

GameScene::Builder& GameScene::Builder::setPlayer(Core::Logic::Color player) noexcept
{
    this->player = player;
    return *this;
}

GameScene::Builder &GameScene::Builder::setBoardView(const UI::Options::Board &bopt) noexcept
{
    this->bopt.emplace(bopt);
    return *this;
}

GameScene GameScene::Builder::build()
{
    if(!window || !bus)
        throw std::invalid_argument("no bus or event in game scene builder");

    if(!fen) 
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    if(!player)
        player = Core::Logic::WHITE;

    if(!bopt) {
        bopt.emplace(UI::Options::Board::Builder()
            .setPlayer(*player)
            .setWindowSize(window->getSize())
            .build()
        );
    }

    return {
        *window, 
        *bus, 
        *fen,
        *player, 
        *bopt
    };
}

void GameScene::RenderImpl()
{
    UIController.Render(window);
}

GameScene::GameScene(
    sf::RenderWindow &window,
    Shared::Bus &bus,
    const std::string &fen,
    Core::Logic::Color player,
    const UI::Options::Board &bopt) noexcept : IScene(window, bus),
                                               UIController(bus),
                                               GameController(bus)
{
    GameController.Init(fen, player);
    UIController.Init(bopt);
    GameController.Start();
}


}