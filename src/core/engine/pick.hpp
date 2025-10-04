#pragma once 

#include "logic/defs.hpp"
#include "logic/move.hpp"
#include "logic/movelist.hpp"
#include "logic/position.hpp"
#include "logic/square.hpp"
#include "logic/attack.hpp"
#include <optional>

namespace Core::Engine 
{

class MovePicker {
public:

    MovePicker(
        const Logic::MoveList&, 
        const Logic::PositionFM&, 
        Logic::Move* killers = nullptr, 
        std::optional<Logic::Move> ttMove = std::nullopt
    );
    std::optional<Logic::Move> next();
    void update(Logic::Move bestMovePrevIter);

private:

    int computeScore(Logic::Move) const;
    int computeCaptureScore(Logic::Square from, Logic::Square targ) const;
    std::optional<Logic::Square> captureTarget(Logic::Move) const;
    template<typename ... Attackers>
    Logic::Bitboard XrayAttacks(Logic::Piece type, const Logic::AttackParams& params, Attackers ... attackers) const {
        return Logic::GetFastAttack(type, params) & pos.GetPieces(Logic::ANY_COLOR, std::forward<Logic::Piece>(attackers)...);
    }
    void updateCapture(Logic::Bitboard& occ, int* gain, Logic::AttackParams& params, Logic::Bitboard least, Logic::Piece victim) const;

private:

    const Logic::PositionFM& pos;
    const Logic::Move* killers;
    Logic::ExtMove moves[Logic::MAX_MOVES_COUNT];
    Logic::ExtMove* cur;
    Logic::ExtMove* end;
    Logic::Move ttMove;

};


}