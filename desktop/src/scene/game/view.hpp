#pragma once

#include "idle.hpp"
#include "controllers/game/view.hpp"
#include "controllers/ui/board/view.hpp"
#include "controllers/ai/view.hpp"

namespace scene
{


class GameScene : public IScene {
public:

    GameScene();
    void Display(sf::RenderWindow& window) override;
    void ParseConfig(const resource::ConfigManager& config) override;
    
private:

    event::Bus bus;
    controller::GameLogicController GameController;
    controller::UIGameController UIController;
    controller::AIController AIController;

};


}