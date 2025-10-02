#pragma once 

#include "SFML/System/Vector2.hpp"
#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"

#include <optional>

namespace UI::Options  
{

class Board {
public:

    Core::Logic::Color player() const noexcept {return player_;}
    sf::Vector2f size() const noexcept {return size_;}
    sf::Vector2f left_bottom() const noexcept {return left_bottom_;}
    float padding() const noexcept {return padding_;}

    class Builder {
    public:

        Builder& setPlayer(Core::Logic::Color) noexcept;
        Builder& setWindowSize(sf::Vector2u) noexcept;
        Builder& setWindowFactor(float);
        Builder& setBoardPaddingFactor(float);
        Board build() const;

    private:

        Core::Logic::Color player;
        std::optional<sf::Vector2u> window_size;
        float window_factor = 0.8f;
        float board_padding_factor = 0.05f;

    };

    Board(const Board& b) noexcept : Board(
        b.player_, 
        b.size_,
         
        b.left_bottom_, 
        b.padding_)
    {}

private:

    Board(
        Core::Logic::Color player,
        sf::Vector2f size,
        sf::Vector2f left_bottom,
        float padding
    ) noexcept : 
        player_(player), 
        size_(size), 
        left_bottom_(left_bottom), 
        padding_(padding)
    {}

private:

    const Core::Logic::Color player_;
    const sf::Vector2f size_;
    const sf::Vector2f left_bottom_;
    const float padding_;

};


class BoardVisual {
public:

    void Init(const Board&);

    sf::Vector2f ToVec(Core::Logic::Square, bool leftBottom = false) const;
    std::optional<Core::Logic::Square> ToSquare(sf::Vector2f) const;

    Core::Logic::Color player() const noexcept {return player_;}
    sf::Vector2f origin() const noexcept {return origin_cell.center;}
    sf::Vector2f cell_size() const noexcept {return origin_cell.size;}

private:

    Core::Logic::Color player_;
    struct Cell {
        sf::Vector2f center;
        sf::Vector2f size;
    } origin_cell;

};


}