#include "view.hpp"

#include "../game/view.hpp"

namespace scene
{


void SceneManager::Load(const resource::ConfigManager &config)
{
    if(!scene) {
        scene = CreateScene<GameScene>();
    }
    scene->ParseConfig(config);
}


}