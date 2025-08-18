#pragma once

#include "bitboard.hpp"
#include "defs.hpp"

#include <optional>
#include <cassert>
#include <memory>


namespace game::logic
{


class AttackParams
{
    using OptBB = std::optional<Bitboard>;
    using OptCLR = std::optional<Color>;
    using OptDIR = std::optional<DirectionType>;
public:
    
    AttackParams& set_attacker(Square from) noexcept {attacker = from; return *this;}
    AttackParams& set_blockers(OptBB bb) noexcept {blockers = bb; return *this;}
    AttackParams& set_color(OptCLR clr) noexcept {color = clr; return *this;}
    AttackParams& set_dir(OptDIR d) noexcept {dir = d; return *this;}

    Square get_attacker() const {return attacker;}
    Bitboard get_blockers() const {return *blockers;}
    Color get_color() const {return *color;}
    DirectionType get_dir() const {return *dir;}

    bool hasAttacker() const noexcept {return attacker.isValid();}
    bool hasBlockers() const noexcept {return blockers.has_value();}
    bool hasColor() const noexcept {return color.has_value();}
    bool hasDir() const noexcept {return dir.has_value();}

private:

    Square attacker{NO_SQUARE};
    OptBB blockers = std::nullopt;
    OptCLR color = std::nullopt;
    OptDIR dir = std::nullopt;

};


/* struct IAttack {
    virtual Bitboard GetSlowAttack(const AttackParams&) const = 0;
    virtual Bitboard GetFastAttack(const AttackParams&) const = 0;
    virtual ~IAttack() = default;
protected:
    virtual void AssertParams(const AttackParams&) const = 0;
};

struct ISlideAttack : virtual IAttack {
protected:
    void AssertParams(const AttackParams&) const override;
};

struct INonSlideAttack : IAttack {
protected:
    void AssertParams(const AttackParams&) const override;
};

struct BishopAttack : virtual ISlideAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
};

struct RookAttack : virtual ISlideAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
};

struct QueenAttack : BishopAttack, RookAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
};

struct KingAttack : INonSlideAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
};

struct KnightAttack : INonSlideAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
};

struct PawnAttack : INonSlideAttack {
    Bitboard GetSlowAttack(const AttackParams&) const override;
    Bitboard GetFastAttack(const AttackParams&) const override;
protected:
    void AssertParams(const AttackParams&) const override;
};

*/


void SetupAttacks();
Bitboard GetFastAttack(Piece pt, const AttackParams& ap);
Bitboard GetSlowAttack(Piece pt, const AttackParams& ap);

}