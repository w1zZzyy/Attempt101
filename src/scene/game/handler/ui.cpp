#include "ui.hpp"

#include "scene/game/event/game-started.hpp"
#include "scene/game/event/mouse.hpp"

namespace Scene::Game::Handler 
{

UIHandler::UIHandler(Shared::Bus& bus) : 
    bus(bus),
    board(opt),
    board_machine(board),
    pieces(opt),
    piece_machine(pieces)
{}

void UIHandler::Init(const UI::Options::Board& bopt)
{
    opt.Init(bopt);
    board.Init(bopt);

    OnGameStarted();
    OnMouseEvent<Shared::Event::MousePressed, Event::MousePressed>();
    OnMouseEvent<Shared::Event::MouseReleased, Event::MouseReleased>();
    OnMouseEvent<Shared::Event::MouseMoved, Event::MouseMoved>();
}

void UIHandler::Render(sf::RenderWindow &window) {
    board.Render(window);
    pieces.Render(window);
}

void UIHandler::OnGameStarted()
{
    using TEvent = Event::GameStarted;
    bus.Subscribe<TEvent>(
        [this](const TEvent& event)
        {
            piece_machine.HandleEvent(event);
            board_machine.HandleEvent(event);
        }
    );
}

template<Model::EventType TEvent, Model::EventType CEvent>
void UIHandler::OnMouseEvent()
{
    bus.Subscribe<TEvent>(
        [this](const TEvent& __event)
        {
            CEvent event{__event}; 
            event.sqr = opt.ToSquare(event.pos);
            
            piece_machine.HandleEvent(event);
            board_machine.HandleEvent(event);
        }
    );
}


}