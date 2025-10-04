#pragma once

#include "scene/model/machine.hpp"
#include "piece/idle.hpp"
#include "piece/in-progress.hpp"
#include "piece/piece-grabbed.hpp"
#include "piece/piece-selected.hpp"
#include "board/idle.hpp"
#include "board/in-progress.hpp"
#include "board/piece-selected.hpp"
#include "promotion/idle.hpp"
#include "promotion/in-progress.hpp"
#include "ui/renderer/board.hpp"
#include "ui/renderer/pieces.hpp"
#include "ui/renderer/promotion.hpp"

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

class PromotionMachine : public Model::Machine<UI::Renderer::Promotion, 
    Promotion::Idle,
    Promotion::InProgressIdle>
{
public:
    PromotionMachine(UI::Renderer::Promotion& object, Shared::Bus& bus) noexcept
        : Machine(object, bus) {}
    
    bool InActive() const {
        return std::get_if<Promotion::InProgressIdle>(&state);
    }
};

}