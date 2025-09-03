#pragma once

#include <vector>

#include "logic/square.hpp"
#include "logic//move.hpp"

namespace ui 
{

class Selected
{
public:

    using Moves = std::vector<game::logic::Move>;

    void SetRights(game::logic::Color) noexcept;
    void SetMoves(Moves&&) noexcept;
    void SetSelected(game::logic::Square) noexcept;

    void Drop() noexcept;
    void Reset() noexcept;

    bool IsSettled() const noexcept;
    bool IsGrabbed() const noexcept;

    bool HasRights(game::logic::Color piece_color) const noexcept;

    const Moves& GetMoves() const noexcept;
    operator game::logic::Square() const noexcept;

private:

    enum class Rights {
        WhitePieces, 
        BlackPieces,
        Sudo = WhitePieces | BlackPieces
    } 
    rights;

    enum class Status {
        Idle = 0, 
        Settled = 0b01, 
        Grabbed = 0b11
    }
    status{Status::Idle};

    game::logic::Square selected_on;
    Moves moves;

};

}