#include "input.hpp"

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

#include "event/mouse.hpp"

namespace Scene::Shared 
{

void Input::handleEvents(sf::RenderWindow& window)
{
    while(std::optional event = window.pollEvent())
    {
        if(event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }

        if (proceed<Event::MousePressed, sf::Event::MouseButtonPressed>(*event)) continue;;
        if (proceed<Event::MouseReleased, sf::Event::MouseButtonReleased>(*event)) continue;;
        if (proceed<Event::MouseMoved, sf::Event::MouseMoved>(*event)) continue;
    }
}

template<Model::EventType TEvent, typename T>
bool Input::proceed(sf::Event& event) 
{
    constexpr bool checkLB = std::is_same_v<TEvent, Event::MousePressed>;

    if(const auto* mouse = event.getIf<T>()) {
        if constexpr (checkLB)
            if(mouse->button != sf::Mouse::Button::Left)
                return false;

        bus.Publish(TEvent{sf::Vector2f(
            float(mouse->position.x), 
            float(mouse->position.y)
        )});
        return true;
    }

    return false;
}

}