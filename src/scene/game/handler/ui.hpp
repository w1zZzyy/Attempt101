#pragma once

#include "scene/model/event.hpp"
#include "scene/shared/bus.hpp"
#include "scene/game/state/machine.hpp"
#include "ui/model/options.hpp"
#include "ui/renderer/board.hpp"
#include "ui/renderer/pieces.hpp"

namespace Scene::Game::Handler 
{

class UIHandler {
public:

    UIHandler(Shared::Bus&);
    void Init(const UI::Options::Board&);
    void Render(sf::RenderWindow&);

private:

    template<Model::EventType TEvent>
    void OnGame();

    template<Model::EventType TEvent, Model::EventType CEvent>
    void OnMouseEvent();

private:

    Shared::Bus& bus;
    UI::Options::BoardVisual opt;
    UI::Renderer::Board board;
    UI::Renderer::Pieces pieces;
    State::PieceMachine piece_machine;
    State::BoardMachine board_machine;

};

}