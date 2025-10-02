#pragma once

#include "scene/model/machine.hpp"
#include "piece/idle.hpp"
#include "piece/in-progress.hpp"
#include "board/idle.hpp"
#include "board/in-progress.hpp"
#include "board/piece-selected.hpp"
#include "ui/renderer/board.hpp"
#include "ui/renderer/pieces.hpp"

namespace Scene::Game::State 
{

class PieceMachine : public Model::Machine<UI::Renderer::Pieces, 
    Piece::Idle,
    Piece::InProgressIdle>
{};

class BoardMachine : public Model::Machine<UI::Renderer::Board, 
    Board::Idle,
    Board::InProgressIdle,
    Board::PieceSelected>
{};

}