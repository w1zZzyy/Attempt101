#include "input.hpp"

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

#include "event/mouse.hpp"
#include "event/window-closed.hpp"

namespace Scene::Shared 
{

void Input::handleEvent(sf::Event& event)
{
    if(event.is<sf::Event::Closed>()) {
        bus.Publish<Event::WindowClosed>({});
        return;
    }

    if (proceed<Event::MousePressed, sf::Event::MouseButtonPressed>(event)) return;
    if (proceed<Event::MouseReleased, sf::Event::MouseButtonReleased>(event)) return;
    if (proceed<Event::MouseMoved, sf::Event::MouseMoved>(event)) return;
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