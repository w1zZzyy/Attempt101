#include "player.hpp"

using namespace game::logic;

std::expected<game::logic::Move, std::string> match::model::Player::DoMove(
    std::string_view notation
) {
    if(side != game->getSide()) {
        return std::unexpected("not player turn to move");
    }

    auto move = game->DoMove(notation);
    if(!move) {
        return std::unexpected(
            "move could not be permutated, wrong notation"
        );
    }

    return move.value();
}
