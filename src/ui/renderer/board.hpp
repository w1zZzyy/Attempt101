#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "resources/text.hpp"

#include <array>
#include <vector>

namespace UI::Renderer 
{

class Board
{
public:

    enum HighLight {
        WhiteCell, 
        BlackCell, 
        Background, 
        Danger, 
        Valid, 
        Selected
    };
    static constexpr int HighLightCnt = 6;

    struct Options {
        sf::Vector2f size;
        sf::Vector2f origin;
        Core::Logic::Color player;
        float padding;
    };

    Board();
    void Init(const Options&);
    void Render(sf::RenderWindow&) const;

private:

    sf::VertexArray background, board;
    Resources::Text textBuilder;
    mutable sf::CircleShape cellBase;

    sf::Color cellColor[HighLightCnt];
    std::array<std::vector<Core::Logic::Square>, HighLightCnt> hl;
    std::array<sf::Vector2f, Core::Logic::SQUARE_COUNT> cords;

};

}