#pragma once

#include "scene/model/machine.hpp"
#include "piece/idle.hpp"
#include "piece/in-progress.hpp"
#include "piece/piece-grabbed.hpp"
#include "piece/piece-selected.hpp"
#include "board/idle.hpp"
#include "board/in-progress.hpp"
#include "board/piece-selected.hpp"
#include "ui/renderer/board.hpp"
#include "ui/renderer/pieces.hpp"

namespace Scene::Game::State 
{

class PieceMachine : public Model::Machine<UI::Renderer::Pieces, 
    Piece::Idle,
    Piece::InProgressIdle, 
    Piece::PieceGrabbed, 
    Piece::PieceSelected>
{
public:
    PieceMachine(UI::Renderer::Pieces& object, Shared::Bus& bus) noexcept
        : Machine(object, bus) {}
};

class BoardMachine : public Model::Machine<UI::Renderer::Board, 
    Board::Idle,
    Board::InProgressIdle,
    Board::PieceSelected>
{
public:
    BoardMachine(UI::Renderer::Board& object, Shared::Bus& bus) noexcept
        : Machine(object, bus) {}
};

}