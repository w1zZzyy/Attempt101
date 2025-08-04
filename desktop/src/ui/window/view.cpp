#include "view.hpp"

namespace desktop::ui
{

WindowRenderer::WindowRenderer()
{
    scene_manager.NextScene(*this);
}

WindowRenderer &WindowRenderer::setWindowSize(unsigned int x, unsigned int y) noexcept
{
    window.create(sf::VideoMode(sf::Vector2u(x, y)), "");
    return *this;
}

WindowRenderer &WindowRenderer::setTitle(const std::string& title) noexcept
{
    window.setTitle(title);
    return *this;
}

WindowRenderer &WindowRenderer::setFPS(unsigned int fps) noexcept
{
    window.setFramerateLimit(fps);
    return *this;
}

void WindowRenderer::Draw()
{
    window.clear();
    scene_manager.Render(window);
    window.display();
}

}