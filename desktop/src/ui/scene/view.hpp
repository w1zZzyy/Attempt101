#pragma once

#include "board/view.hpp"
#include "piece/view.hpp"
#include "resources/config_manager/view.hpp"
#include "logic/src/position.hpp"
#include "logic/src/movelist.hpp"

#include <vector>

namespace ui
{


class IScene
{
public:
    virtual ~IScene() = default;
    virtual void Render(sf::RenderWindow&) = 0;
    virtual void ReadConfig(const resource::BoardConfigManager&) = 0;
};


class GameScene : public IScene {
public:

    GameScene(game::logic::Color side) : player_side(side) {}
    void Render(sf::RenderWindow&) override;
    void ReadConfig(const resource::BoardConfigManager&) override;

private:

    void update_game();

private:

    game::logic::Color player_side;

    BoardRenderer board;
    std::vector<EntityPtr> entities;

    game::logic::Position pos;
    game::logic::MoveList legal_moves;
    game::logic::GameStatus status;

};


class SceneManager
{
public:

    SceneManager();
    void Render(sf::RenderWindow& window) {scene->Render(window);};

private:

    std::unique_ptr<IScene> scene;

};


}