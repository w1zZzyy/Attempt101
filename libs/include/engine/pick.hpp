#pragma once 

#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"
#include "logic/position.hpp"
#include "logic/square.hpp"
#include <optional>

namespace game::engine 
{


class MovePicker {
public:

    MovePicker(const logic::MoveList&, const PositionFixedMemory&, std::optional<logic::Move> = std::nullopt);
    std::optional<logic::Move> next();

private:

    int computeScore(logic::Move) const;
    int computeCaptureScore(logic::Square from, logic::Square targ) const;
    std::optional<logic::Square> captureTarget(logic::Move) const;
    template<typename ... Attackers>
    logic::Bitboard XrayAttacks(logic::Piece type, const logic::AttackParams& params, Attackers ... attackers) const {
        return logic::GetFastAttack(type, params) & pos.get_pieces(logic::ANY_COLOR, std::forward<logic::Piece>(attackers)...);
    }
    void updateCapture(logic::Bitboard& occ, int* gain, logic::AttackParams& params, logic::Bitboard least, logic::Piece victim) const;

private:

    const PositionFixedMemory& pos;
    logic::ExtMove moves[logic::MAX_MOVES_COUNT];
    logic::ExtMove* cur;
    logic::ExtMove* end;
    logic::Move ttMove;

};


}