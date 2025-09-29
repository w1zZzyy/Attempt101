#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include "../resources/text.hpp"
#include "../resources/colors.hpp"
#include "../model/options.hpp"

namespace UI::Renderer 
{

class Board
{
public:

    Board(const Options::BoardVisual& opt);
    void Init(const Options::Board&);
    void Render(sf::RenderWindow&) const;

private:

    const Options::BoardVisual& opt;
    sf::VertexArray background, board;
    Resources::Text textBuilder;
    Resources::Colors colorBuilder;

};

}