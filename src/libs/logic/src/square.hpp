#pragma once

#include <cstdint>
#include <cassert>
#include <ostream>
#include <sstream>
#include <string_view>

namespace game::logic
{


class Bitboard;


enum SquareType : uint8_t
{
    a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
    NO_SQUARE
};

class Square
{
public:

    static void Setup();
    static constexpr SquareType Start() noexcept {return a1;};
    static constexpr SquareType End() noexcept {return h8;};
    static constexpr SquareType ToSquare(int index) {
        if(index > -1 && index < NO_SQUARE)
            return static_cast<SquareType>(index);
        return NO_SQUARE;
    }
    static SquareType ToSquare(std::string_view index) {
        auto file = index[0] - 'a';
        auto rank = index[1] - '1';
        return ToSquare(rank * 8 + file);
    }

    constexpr Square()                noexcept : index(NO_SQUARE) {}
    constexpr Square(SquareType i)    noexcept : index(i) {}
    constexpr Square(const Square& s) noexcept : index(s.index) {}
    constexpr Square(int i) : index(ToSquare(i)) {}

    Square& operator = (const Square& s) noexcept {index = s.index; return *this;}
    Square& operator = (SquareType s) noexcept {index = s; return *this;}

    Square& operator ++ () noexcept {index = ToSquare(index + 1); return *this;}
    Square& operator -- () noexcept {index = ToSquare(index - 1); return *this;}


    constexpr bool operator < (const Square& s) const noexcept {return index < s.index;}
    constexpr bool operator > (const Square& s) const noexcept {return index > s.index;}
    constexpr bool operator < (int i) const noexcept {return index < i;}
    constexpr bool operator > (int i) const noexcept {return index > i;}
    bool operator == (const Square& s) const noexcept {return index == s.index;}
    bool operator != (const Square& s) const noexcept {return index == s.index;}
    bool operator == (SquareType st) const noexcept {return index == st;}
    bool operator != (SquareType st) const noexcept {return index != st;}


    Square operator + (int i) const {return ToSquare(index + i);}
    Square operator - (int i) const {return ToSquare(index - i);}
    Square& operator += (int i) {index = SquareType(index + i); return *this;}
    Square& operator -= (int i) {index = SquareType(index - i); return *this;}

    constexpr int operator / (int num) const noexcept {return index / num;}
    constexpr int operator % (int num) const noexcept {return index % num;}

    constexpr operator int() const noexcept {return index;}
    
    constexpr bool isValid() const noexcept {return index != NO_SQUARE;}

    Bitboard bitboard() const noexcept;

    std::string to_string() const noexcept {
        std::ostringstream os;
        os << char('a' + index % 8) << index / 8 + 1;
        return os.str();
    }

private:

    SquareType index;

};


// (sq1; sq2]
Bitboard between(Square sq1, Square sq2);

// (c2, e4) -> [b1; h7]
Bitboard line_bb(Square sq1, Square sq2);

}


std::ostream& operator << (std::ostream& out, const game::logic::Square& sqr);
