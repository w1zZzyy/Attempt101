#pragma once

#include "event/bus/view.hpp"
#include "manager/logic.hpp" 

namespace controller
{


class GameManager
{
public:

    GameManager(event::Bus& bus);
    void Init(const std::string& fen);
    void Update() const;

private:

    void SubscribeOnMoveEvent();
    void HandleMoveError(const event::MoveEvent& event, game::LogicException err) const;
    void HandleMove(game::logic::Move move);

    void SubscribeOnBoardClickedEvent();
    void MaybePieceClick(game::logic::Square sqr) const;

private:

    event::Bus& bus;
    game::LogicManager manager;

};


}