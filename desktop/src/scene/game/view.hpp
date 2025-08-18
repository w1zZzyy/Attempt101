#pragma once

#include "idle.hpp"
#include "controllers/game/view.hpp"
#include "controllers/ui/board/view.hpp"
#include "controllers/input/view.hpp"

namespace scene
{


class GameScene : public IScene {
public:

    GameScene();
    void Display(sf::RenderWindow& window) override;
    void ParseConfig(const resource::ConfigManager& config) override;
    void HandleEvents(sf::RenderWindow& window) override;

private:

    event::Bus bus;
    controller::GameManager GameController;
    controller::UIBoardManager UIController;
    controller::InputManager InputController;

};


}