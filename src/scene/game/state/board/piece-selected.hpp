#pragma once

#include "ui/renderer/board.hpp"
#include "../in-progress.hpp"

namespace Scene::Game::State::Board 
{

class PieceSelected : public InProgress<PieceSelected, UI::Renderer::Board>
{
};

}