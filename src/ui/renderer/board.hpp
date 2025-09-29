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

    Board();
    void Init(const Model::Options&);
    void Render(sf::RenderWindow&) const;

private:

    sf::VertexArray background, board;
    Resources::Text textBuilder;
    Resources::Colors colorBuilder;

};

}