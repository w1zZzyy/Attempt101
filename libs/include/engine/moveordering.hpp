#pragma once 

#include "logic/movelist.hpp"
#include "logic/attack.hpp"

namespace game::engine 
{

class MoveOrderer {
public:

    void setPosition(const PositionFixedMemory& p) noexcept {pos = &p;}
    void OrderCaptures(logic::MoveList& moves) const;

private:

    int CaptureScore(logic::Move move) const;
    
    template<typename ... Attackers>
    logic::Bitboard XrayAttacks(logic::Piece type, const logic::AttackParams& params, Attackers ... attackers) const {
        return logic::GetFastAttack(type, params) & pos->get_pieces(logic::ANY_COLOR, std::forward<logic::Piece>(attackers)...);
    }

    void UpdateCapture(logic::Bitboard& occ, int* gain, logic::AttackParams& params, logic::Bitboard least) const;

private:

    const PositionFixedMemory* pos;

};



}