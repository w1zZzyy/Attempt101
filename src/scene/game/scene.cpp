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

GameScene::Builder &GameScene::Builder::setEngineOptions(const Core::Engine::Search::Options& eopt) noexcept
{
    this->eopt = eopt;
    return *this;
}

GameScene GameScene::Builder::build()
{
    if(!window || !bus || !bopt)
        throw std::invalid_argument("no bus or event or bopt in game scene builder");

    return {
        *window, 
        *bus, 
        fen,
        player, 
        *bopt, 
        eopt
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
    const UI::Options::Board &bopt, 
    const Core::Engine::Search::Options& eopt) noexcept : IScene(window, bus),
                                               UIController(bus),
                                               GameController(bus), 
                                               AIController(bus)
{
    GameController.Init(fen, player);
    UIController.Init(bopt);
    AIController.Init(eopt);

    GameController.Start();
    AIController.Start();
}


}