#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../resources/text.hpp"
#include "../model/options.hpp"
#include "../model/highlight.hpp"

#include <array>
#include <vector>

namespace UI::Renderer 
{

class Board
{
public:

    Board();
    void Init(const Model::Options&);
    void Render(sf::RenderWindow&) const;

private:

    sf::VertexArray background, board;
    Resources::Text textBuilder;
    mutable sf::CircleShape cellBase;

    sf::Color cellColor[Model::HighLightCnt];
    std::array<std::vector<Core::Logic::Square>, Model::HighLightCnt> hl;
    std::array<sf::Vector2f, Core::Logic::SQUARE_COUNT> cords;

};

}