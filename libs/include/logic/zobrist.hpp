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

    operator U64() const noexcept {return hash;}
    bool operator == (const Zobrist& z) const noexcept {return hash == z.hash;}

private:

    U64 hash {0};

};

}