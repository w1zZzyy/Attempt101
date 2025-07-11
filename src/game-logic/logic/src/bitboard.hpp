#pragma once

#include <cstdint>
#include <type_traits>

#include "square.hpp"
#include "defs.hpp"

namespace game::logic
{


class Bitboard
{
public:

    static Bitboard Null() noexcept {return Bitboard(0ULL);}
    static Bitboard Full() noexcept {return ~Null();}
    template<typename... Squares>
    static Bitboard FromSquares(Squares... sqr) {
        constexpr bool isSquare     = (std::is_same_v<Square, Squares> && ...);
        constexpr bool isSquareType = (std::is_same_v<SquareType, Squares> && ...);
        static_assert((isSquare || isSquareType), "All arguments must be of type Square or SquareType");

        Bitboard b;

        if constexpr (isSquare)
            b.bb = (sqr.bitboard() | ...);
        if constexpr (isSquareType)
            b.bb = (Square(sqr).bitboard() | ...);

        return b;
    }

    Bitboard()                  noexcept : bb(0) {}
    Bitboard(const Bitboard& b) noexcept : bb(b.bb) {}
    Bitboard(U64 bb)            noexcept : bb(bb) {}

    Bitboard& operator  =   (const Bitboard& b) noexcept {bb = b.bb; return *this;}
    Bitboard& operator  |=  (const Bitboard& b) noexcept {bb |= b.bb; return *this;}
    Bitboard& operator  &=  (const Bitboard& b) noexcept {bb &= b.bb; return *this;}
    Bitboard& operator  <<= (int num) noexcept {bb <<= num; return *this;}
    Bitboard& operator  >>= (int num) noexcept {bb >>= num; return *this;}
    Bitboard& operator  ^=  (const Bitboard& b) noexcept {bb ^= b.bb; return *this;}
    Bitboard operator   |   (const Bitboard& b) noexcept {return Bitboard(*this) |= b;}
    Bitboard operator   &   (const Bitboard& b) noexcept {return Bitboard(*this) &= b;}
    Bitboard operator   <<  (int num) noexcept {return Bitboard(*this) <<= num;}
    Bitboard operator   >>  (int num) noexcept {return Bitboard(*this) >>= num;}
    Bitboard operator   ~   () const noexcept {return Bitboard(~bb);}
    Bitboard operator   ^   (const Bitboard& b) const noexcept {return Bitboard(*this) ^= b;}
    bool operator       ==  (const Bitboard& b) const noexcept {return bb == b.bb;}
    bool operator       !=  (const Bitboard& b) const noexcept {return bb != b.bb;}

    template<DirectionType dir>
    Bitboard step() noexcept;

    operator bool() const noexcept {return bb != 0;}
    bool operator ! () const noexcept {return bb == 0;}
    
    Square lsb() const;
    Square poplsb();

    int count() const;

    friend int pext(const Bitboard& blockers, const Bitboard& attacks);
    friend Bitboard pdep(int num, const Bitboard& attacks);

private:

    U64 bb;

};


int pext(const Bitboard& blockers, const Bitboard& attacks);
Bitboard pdep(int num, const Bitboard& attacks);


}