#pragma once

#include "SFML/Graphics.hpp"
#include <memory>

namespace ui::model
{

class Entity
{
public:
    virtual ~Entity() = default;
    virtual void Render(sf::RenderWindow&) = 0;
    virtual Entity& setSize(sf::Vector2f) = 0;
    virtual Entity& setPos(sf::Vector2f) = 0;

};  

using EntityPtr = std::unique_ptr<Entity>;

template<typename EntityChild, typename ... Args>
EntityPtr CreateEntity(Args&& ... args) {
    return std::make_unique<EntityChild>(
        std::forward<Args>(args)...
    );
}

}