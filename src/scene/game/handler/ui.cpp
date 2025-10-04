#include "ui.hpp"

#include "scene/game/event/game-started.hpp"
#include "scene/game/event/mouse.hpp"

namespace Scene::Game::Handler 
{

UIHandler::UIHandler(Shared::Bus& bus) : 
    bus(bus),
    board(opt),
    board_machine(board, bus),
    pieces(opt),
    piece_machine(pieces, bus), 
    promo(opt), 
    promo_machine(promo, bus)
{}

void UIHandler::Init(const UI::Options::Board& bopt)
{
    opt.Init(bopt);
    board.Init(bopt);
    promo.Init();

    OnGame<Event::GameStarted>();
    OnGame<Event::GameUpdated>();
    OnMouseEvent<Shared::Event::MousePressed, Event::MousePressed>();
    OnMouseEvent<Shared::Event::MouseReleased, Event::MouseReleased>();
    OnMouseEvent<Shared::Event::MouseMoved, Event::MouseMoved>();
    OnPromotion();
}

void UIHandler::Render(sf::RenderWindow &window) {
    board.Render(window);
    pieces.Render(window);
    promo.Render(window);
}

void UIHandler::OnPromotion() 
{
    bus.Subscribe<Event::Promotion>(
        [this](const Event::Promotion& event) {
            promo_machine.HandleEvent(event);
            board_machine.HandleEvent(event);
        }
    );
}

template<Model::EventType TEvent>
void UIHandler::OnGame()
{
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

             if(promo_machine.InActive()) {
                promo_machine.HandleEvent(event);
                return;
            }

            event.sqr = opt.ToSquare(event.pos);
            
            piece_machine.HandleEvent(event);
            board_machine.HandleEvent(event);
        }
    );
}


}