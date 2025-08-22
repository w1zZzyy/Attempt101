#include "logic.hpp"

namespace game
{


void LogicManager::Init(const std::string &fen)
{
    PositionDynamicMemory::Init();

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

    std::vector<logic::Move> candidates;
    candidates.reserve(8);

    for(size_t i = 0; i < legal_moves.get_size(); ++i)
    {
        const auto m = legal_moves[i]; 
        if (m.from() == from && m.targ() == targ) 
            candidates.push_back(m);
    }

    if (candidates.empty())
        return std::unexpected(LogicException::MoveNotFound);

    if(candidates.size() == 1) {
        DoMove(candidates[0]);
        return candidates[0];
    }
    
    if(!flag) 
        return std::unexpected(LogicException::PromotionFlagNeeded);

    if(
        auto it = std::ranges::find_if(candidates, [&](const logic::Move& m) {
            return m.flag() == *flag;
        });
        it != candidates.end() 
    ) { 
        DoMove(*it);
        return *it; 
    }

    return std::unexpected(LogicException::MoveNotFound);
}

void LogicManager::UpdateStatus()
{
    pos.update();
    legal_moves.generate(pos);

    if(legal_moves.empty()) {
        status = pos.is_check()
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