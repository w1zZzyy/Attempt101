#include "view.hpp"

#include "game/view.hpp"

namespace scene
{

void SceneManager::Handle(sf::RenderWindow &window)
{
    while(window.isOpen())
    {
        scene->PublishEvents(window);
        scene->Display(window);
    }
}

void SceneManager::Load(const resource::ConfigManager &config)
{
    if(!scene) {
        scene = CreateScene<GameScene>();
    }
    scene->ParseConfig(config);
}


}