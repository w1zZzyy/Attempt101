#pragma once

#include "square.hpp"
#include "defs.hpp"

namespace game::logic
{


class Zobrist
{
public:

    static void Setup();
    
    Zobrist& updateSquare(Color c, Piece p, Square s);
    Zobrist& updateCastle(Castle cr);
    Zobrist& updateEnPassant(Square sqr);
    Zobrist& updateSide();

    bool operator == (const Zobrist& z) const noexcept {return hash == z.hash;}

private:

    Bitboard hash {0};

};


}