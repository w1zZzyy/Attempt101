#pragma once 

#include "logic/defs.hpp"
#include "logic/position.hpp"

namespace Core::Engine
{

class Evaluation {
public:

    static void Setup();

    void Init(const Logic::PositionFM&);
    void Update(Logic::Move);
    void Rollback();
    int Score() const;

private:

    struct Data {
        int mg[Logic::COLOR_COUNT];
        int eg[Logic::COLOR_COUNT];
        int game_phase;
    };

private:

    void addPiece(Logic::Color side, Logic::Piece piece, Logic::Square sqr);
    void removePiece(Logic::Color side, Logic::Piece piece, Logic::Square from);
    void movePiece(Logic::Color side, Logic::Piece piece, Logic::Square from, Logic::Square targ);
    void castle(Logic::Color side, Logic::Square kf, Logic::Square kt, Logic::Square rf, Logic::Square rt);

private:

    Data data[Logic::MAX_HISTORY_SIZE];
    Data* cur = data;
    const Logic::PositionFM* pos;

};

}