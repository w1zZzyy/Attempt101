#include "options.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

#include <stdexcept>

namespace UI::Options 
{

namespace 
{

void checkFactor(float factor) {
    if(factor <= 0 || factor > 1) {
        throw std::invalid_argument(
            R"(
            factor must be (0, 1].
            could not build Board
            )"
        );
    }
}

}

Board::Builder& Board::Builder::setPlayer(Core::Logic::Color player) noexcept
{
    this->player = player;
    return *this;
}

Board::Builder& Board::Builder::setWindowSize(sf::Vector2u window_size) noexcept
{
    this->window_size = window_size;
    return *this;
}

Board::Builder& Board::Builder::setWindowFactor(float factor)
{
    checkFactor(factor);
    this->window_factor = factor;
    return *this;
}

Board::Builder& Board::Builder::setBoardPaddingFactor(float factor)
{
    checkFactor(factor);
    this->board_padding_factor = factor;
    return *this;
}

Board Board::Builder::build() const
{
    if(!window_size || !player.isValid()) {
        throw std::invalid_argument(
            R"(
            window size or player is not settled,
            impossible to build Board object
            )"
        );
    }

    const float boardSizeFull = window_factor * std::min(
        window_size->x, 
        window_size->y
    );
    const float boardPadding = board_padding_factor * boardSizeFull;

    const sf::Vector2f boardLB = sf::Vector2f{
        (window_size->x - boardSizeFull) / 2, 
        (window_size->y + boardSizeFull) / 2
    };

    return Board{
        player, 
        {boardSizeFull, boardSizeFull},
        boardLB, 
        boardPadding
    };
}

void BoardVisual::Init(const Board& board)
{
    const sf::Vector2f boardFullSize = board.size();
    const sf::Vector2f boardLB = board.left_bottom();
    const float padding = board.padding();

    origin_cell.size = sf::Vector2f{
        (boardFullSize.x - 2 * padding) / 8.0f,
        (boardFullSize.y - 2 * padding) / 8.0f
    };

    origin_cell.center = sf::Vector2f(
        boardLB.x + padding + origin_cell.size.x / 2,
        boardLB.y - padding - origin_cell.size.y / 2
    );

    player_ = board.player();
} 

sf::Vector2f BoardVisual::ToVec(Core::Logic::Square sqr, bool leftBottom) const
{
    int rank = sqr.rank();
    int file = sqr.file();

    if(player_.is(Core::Logic::BLACK)) {
        rank = 7 - rank;
        file = 7 - file;
    }

    sf::Vector2f vec = sf::Vector2f{
        origin_cell.center.x + origin_cell.size.x * file,
        origin_cell.center.y - origin_cell.size.y * rank,
    };

    if(leftBottom) {
        vec.x -= origin_cell.size.x / 2;
        vec.y += origin_cell.size.y / 2;
    }

    return vec;
}

std::optional<Core::Logic::Square> BoardVisual::ToSquare(sf::Vector2f pos) const
{
    sf::RectangleShape cell;
    cell.setSize(origin_cell.size);
    cell.setOrigin(cell.getLocalBounds().getCenter());

    using namespace Core::Logic;
    
    for(Square sqr = Square::Start(); sqr <= Square::End(); ++sqr) {
        cell.setPosition(ToVec(sqr));
        if(cell.getGlobalBounds().contains(pos)) 
            return sqr;
    }

    return std::nullopt;
}


}