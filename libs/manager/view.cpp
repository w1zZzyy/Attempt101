#include "view.hpp"

namespace game
{


void LogicManager::Init(const std::string &fen)
{
    logic::Position::Init();

    pos.set_fen(fen);
    UpdateStatus();
}

std::vector<logic::Move> LogicManager::MovesFrom(logic::Square sqr) const
{
    std::vector<logic::Move> result;

    for(size_t i = 0; i < legal_moves.get_size(); ++i) {
        if(legal_moves[i].from() == sqr) 
            result.push_back(legal_moves[i]);
    }

    return result;
}

std::expected<logic::Move, LogicException> LogicManager::DoMove(
    logic::Square from, logic::Square targ,
    std::optional<logic::MoveFlag> flag
) {
    if(status != logic::GameStatus::InProgress) 
        return std::unexpected(LogicException::GameStatusError);

    std::optional<logic::Move> move;
    for(size_t i = 0; i < legal_moves.get_size(); ++i)
    {
        logic::Move m = legal_moves[i];
        if(m.from() == from && m.targ() == targ)
        {
            if(flag) { if(*flag == m.flag()) { DoMove(m); return m; }}        
            else if(!move) move = m; 
            else return std::unexpected(LogicException::PromotionFlagNeeded);
        }
    }

    if(!move)
        return std::unexpected(LogicException::MoveNotFound);

    DoMove(*move);
    return *move;
}

void LogicManager::UpdateStatus()
{
    pos.compute_enemy_attackers().compute_pins_from_sliders();
    legal_moves.generate(pos);

    if(pos.is_check()) {
        status = legal_moves.empty() 
        ?  static_cast<logic::GameStatus>(int(pos.get_side().opp()))
        : logic::GameStatus::Draw;
    } else if(pos.is_draw()) {
        status = logic::GameStatus::Draw;
    } else {
        status = logic::GameStatus::InProgress;
    }
}

void LogicManager::DoMove(logic::Move move)
{
    pos.do_move(move);
    UpdateStatus();
}
}