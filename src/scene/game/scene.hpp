#pragma once 

#include "scene/model/scene.hpp"
#include "handler/game.hpp"
#include "handler/ui.hpp"
#include "handler/ai.hpp"

namespace Scene 
{

class GameScene : public Model::IScene<GameScene> {
public:

    class Builder {
    public:

        Builder& setWindow(sf::RenderWindow&) noexcept;
        Builder& setEventBus(Shared::Bus&) noexcept;
        Builder& setFen(std::string) noexcept;
        Builder& setPlayer(Core::Logic::Color) noexcept;
        Builder& setBoardView(const UI::Options::Board&) noexcept;
        Builder& setEngineOptions(Core::Engine::Search::Options&&) noexcept;
        GameScene build();
        
    private:
        sf::RenderWindow* window;
        Shared::Bus* bus;
        std::optional<std::string> fen;
        std::optional<Core::Logic::Color> player;
        std::optional<UI::Options::Board> bopt;
        std::optional<Core::Engine::Search::Options> eopt;
    };

    void RenderImpl();

private:

    GameScene(
        sf::RenderWindow& window, 
        Shared::Bus& bus, 
        const std::string& fen, 
        Core::Logic::Color player, 
        const UI::Options::Board& bopt, 
        const Core::Engine::Search::Options& eopt
    ) noexcept;

private:

    Game::Handler::UIHandler UIController;
    Game::Handler::GameHandler GameController;
    Game::Handler::AIHandler AIController;

};

}