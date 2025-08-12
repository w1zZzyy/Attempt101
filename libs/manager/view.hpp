#pragma once

#include "logic/src/position.hpp"
#include "logic/src/movelist.hpp"
#include <vector>
#include <expected>

namespace game
{

enum class LogicException {
    PromotionFlagNeeded, 
    MoveNotFound,
    GameStatusError
};

class LogicManager
{
public:

    void Init(const std::string& fen);
    logic::Piece getPieceOn(logic::Square sqr) const {return pos.piece_on(sqr);}
    logic::Color getPieceClr(logic::Square sqr) const {return pos.piece_clr_on(sqr);}
    logic::GameStatus getStatus() const noexcept {return status;}
    logic::Piece getCaptured() const {return pos.get_captured();}
    std::vector<logic::Move> MovesFrom(logic::Square sqr) const;

    std::expected<logic::Move, LogicException> DoMove(
        logic::Square from, 
        logic::Square targ,
        std::optional<logic::MoveFlag> flag = std::nullopt
    );

private:

    void UpdateStatus();
    void DoMove(logic::Move move);

private:

    PositionDynamicMemory pos;
    logic::MoveList legal_moves;
    logic::GameStatus status;

};


}