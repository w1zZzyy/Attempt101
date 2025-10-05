#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/VideoMode.hpp"

#include "core/logic/defs.hpp"
#include "scene/game/scene.hpp"
#include "scene/shared/bus.hpp"
#include "scene/shared/input.hpp"

int main()
{
    const auto player = Core::Logic::WHITE;

    Scene::Shared::Bus bus;
    Scene::Shared::Input input(bus);

    sf::RenderWindow Window(sf::VideoMode::getDesktopMode(), "Final Attempt", sf::Style::Titlebar | sf::Style::Close);
    Window.setPosition({0,0});
    Window.setVerticalSyncEnabled(true);

    Scene::GameScene Scene = Scene::GameScene::Builder()
        .setWindow(Window)
        .setEventBus(bus)
        .setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        .setPlayer(player)
        .build();

    bus.Launch();

    while(Window.isOpen())
    {
        input.handleEvents(Window);
        Scene.Render();
    }
}