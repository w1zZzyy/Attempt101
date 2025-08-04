#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "board/view.hpp"

namespace desktop::ui
{

class WindowRenderer;

class IScene {
public:
    virtual ~IScene() = default;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void SetupWindow(WindowRenderer& window) = 0;
};

class GameScene : public IScene {
public:

    GameScene(game::logic::Color side_pov);
    void Render(sf::RenderWindow& window) override;
    void SetupWindow(WindowRenderer& window) override;

private:

    void InitBoard();

private:

    BoardRenderer board;
    game::logic::Color side_pov;

};


class SceneManager {
public:
    void NextScene(WindowRenderer& window);
    void Render(sf::RenderWindow& window) {scene->Render(window);}
private:
    std::unique_ptr<IScene> scene;
};


}