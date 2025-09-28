#pragma once

#include "scene/model/machine.hpp"
#include "piece/idle.hpp"
#include "piece/in-progress.hpp"
#include "ui/renderer/pieces.hpp"

namespace Scene::Game::State 
{

class PieceMachine : public Model::Machine<UI::Renderer::Pieces, 
    Piece::Idle,
    Piece::InProgress>
{};

}