#pragma once 

#include "logic/defs.hpp"
#include "logic/position.hpp"

namespace game::engine
{

class Evaluation {
public:

    static void Setup();

    void Init(const PositionFixedMemory&);
    void Update(logic::Move, logic::Color us);
    int Score() const;

    struct Data {
        int mg[logic::COLOR_COUNT];
        int eg[logic::COLOR_COUNT];
        int game_phase;
    };
    Data GetData() const noexcept {return data;}
    void SetData(const Data& _data) noexcept {data = _data;}

private:

    void addPiece(logic::Color side, logic::Piece piece, logic::Square sqr);
    void removePiece(logic::Color side, logic::Piece piece, logic::Square from);
    void movePiece(logic::Color side, logic::Piece piece, logic::Square from, logic::Square targ);
    void castle(logic::Color side, logic::Square kf, logic::Square kt, logic::Square rf, logic::Square rt);

private:

    Data data;
    const PositionFixedMemory* pos;

};

}