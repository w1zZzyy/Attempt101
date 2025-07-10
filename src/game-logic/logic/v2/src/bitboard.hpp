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

    Bitboard static constexpr Null() noexcept {return 0ULL;}
    Bitboard static constexpr Full() noexcept {return ~Null();}

    Bitboard()                  noexcept : bb(0) {}
    Bitboard(const Bitboard& b) noexcept : bb(b.bb) {}
    Bitboard(U64 bb)            noexcept : bb(bb) {}
    template<typename... Squares>
    Bitboard(Squares... sqr) {
        static_assert(std::is_same_v<Square, Squares> && ...);
        return (sqr.bitboard() | ...);
    }

    Bitboard& operator  =   (const Bitboard& b) noexcept {bb = b.bb;}
    Bitboard& operator  |=  (const Bitboard& b) noexcept {bb |= b.bb;}
    Bitboard& operator  &=  (const Bitboard& b) noexcept {bb &= b.bb;}
    Bitboard& operator  <<= (int num) noexcept {bb <<= num;}
    Bitboard& operator  >>= (int num) noexcept {bb >>= num;}
    Bitboard& operator  ^=  (const Bitboard& b) noexcept {bb ^= b.bb;}
    Bitboard operator   |   (const Bitboard& b) noexcept {return Bitboard(*this) |= b;}
    Bitboard operator   &   (const Bitboard& b) noexcept {return Bitboard(*this) &= b;}
    Bitboard operator   <<  (int num) noexcept {return Bitboard(*this) <<= num;}
    Bitboard operator   >>  (int num) noexcept {return Bitboard(*this) >>= num;}
    Bitboard operator   ~   () const noexcept {return Bitboard(~bb);}
    Bitboard operator   ^   (const Bitboard& b) const noexcept {return Bitboard(*this) ^= b;}

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


}