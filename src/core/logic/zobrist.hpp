#pragma once

#include "square.hpp"
#include "defs.hpp"

namespace Core::Logic
{

class Zobrist
{
public:

    static void Setup();

    Zobrist(U64 h = 0) noexcept : hash(h) {}
    
    Zobrist& updateSquare(Color, Piece, Square) noexcept;
    Zobrist& updateCastle(Castle) noexcept;
    Zobrist& updateEnPassant(Square) noexcept;
    Zobrist& updateSide();

    operator U64() const noexcept {return hash;}
    bool operator == (const Zobrist& z) const noexcept {return hash == z.hash;}

private:

    U64 hash;

};

}