#pragma once

#include <cstdint>
#include <type_traits>

#include "square.hpp"


namespace Core::Logic
{


class Bitboard
{
public:

    static constexpr Bitboard Null() noexcept {return Bitboard(0ULL);}
    static constexpr Bitboard Full() noexcept {return ~Null();}
    template<typename... Squares>
    static constexpr Bitboard FromSquares(Squares... sqr) {
        constexpr bool isSquare     = (std::is_same_v<Square, Squares> && ...);
        constexpr bool isSquareType = (std::is_same_v<SquareType, Squares> && ...);
        static_assert((isSquare || isSquareType), "All arguments must be of type Square or SquareType");
        return Bitboard(((1ULL << sqr) | ...));
    }

    constexpr Bitboard()                    noexcept : bb(0) {}
    constexpr Bitboard(uint64_t bb)         noexcept : bb(bb) {}
    constexpr Bitboard(const Bitboard& b)   noexcept : bb(b.bb) {}

    Bitboard& operator  =   (const Bitboard& b) noexcept {bb = b.bb; return *this;}
    Bitboard& operator  |=  (const Bitboard& b) noexcept {bb |= b.bb; return *this;}
    Bitboard& operator  &=  (const Bitboard& b) noexcept {bb &= b.bb; return *this;}
    Bitboard& operator  <<= (int num) noexcept {bb <<= num; return *this;}
    Bitboard& operator  >>= (int num) noexcept {bb >>= num; return *this;}
    Bitboard& operator  ^=  (const Bitboard& b) noexcept {bb ^= b.bb; return *this;}

    constexpr Bitboard operator   |   (const Bitboard& b) const noexcept {return Bitboard(bb | b.bb);}
    constexpr Bitboard operator   &   (const Bitboard& b) const noexcept {return Bitboard(bb & b.bb);}
    constexpr Bitboard operator   <<  (int num) const noexcept {return Bitboard(bb << num);}
    constexpr Bitboard operator   >>  (int num) const noexcept {return Bitboard(bb >> num);}
    constexpr Bitboard operator   ~   () const noexcept {return Bitboard(~bb);}
    constexpr Bitboard operator   ^   (const Bitboard& b) const noexcept {return Bitboard(bb ^ b.bb);}

    bool operator == (const Bitboard& b) const noexcept {return bb == b.bb;}
    bool operator != (const Bitboard& b) const noexcept {return bb != b.bb;}

    constexpr operator bool() const noexcept {return bb != 0;}
    constexpr bool operator ! () const noexcept {return bb == 0;}
    
    Square lsb() const;
    Square poplsb();

    int count() const;

    friend int pext(const Bitboard& blockers, const Bitboard& attacks);
    friend Bitboard pdep(int num, const Bitboard& attacks);

    void print() const;

private:

    uint64_t bb;

};


int pext(const Bitboard& blockers, const Bitboard& attacks);
Bitboard pdep(int num, const Bitboard& attacks);


}