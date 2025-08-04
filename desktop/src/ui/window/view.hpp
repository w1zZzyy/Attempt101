#pragma once

#include <string>
#include <scene/view.hpp>
#include <optional>

namespace desktop::ui
{

class WindowRenderer
{
public:

    WindowRenderer();

    WindowRenderer& setWindowSize(unsigned int x, unsigned int y) noexcept;
    WindowRenderer& setTitle(const std::string& title) noexcept;
    WindowRenderer& setFPS(unsigned int fps) noexcept;

    void Draw();
    void Close() {window.close();};
    bool IsOpen() const noexcept {return window.isOpen();}

    std::optional<sf::Event> PollEvent() {return window.pollEvent();};

private:

    sf::RenderWindow window;
    SceneManager scene_manager;

};

}