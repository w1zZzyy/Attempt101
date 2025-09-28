#pragma once 

#include "scene/model/state.hpp"
#include "ui/renderer/pieces.hpp"
#include "../../event/game-started.hpp"

namespace Scene::Game::State::Piece
{

class Idle : public Model::IState<Idle, UI::Renderer::Pieces> {
public:
    void HandleEventImpl(const Event::GameStarted& event);
};

}