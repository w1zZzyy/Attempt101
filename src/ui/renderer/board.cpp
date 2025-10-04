#include "board.hpp"

#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../resources/colors.hpp"
#include "../utils/quad.hpp"

#include <string_view>

namespace UI::Renderer 
{

Board::Board(const Options::BoardVisual& opt) : 
    opt(opt),
    textBuilder(ASSETS_PATH "/board/font.ttf"),
    colorBuilder(ASSETS_PATH "/board/colors.csv") 
{}

void Board::Init(const Options::Board& bopt)
{
    // 1. инициализируем статичные вершины

    // 1.1 background

    background.setPrimitiveType(sf::PrimitiveType::Triangles);
    Utils::AppendQuad(
        bopt.left_bottom(), bopt.size(), 
        colorBuilder.Extract<Resources::BACKGROUND_COLOR>(), 
        background
    );

    // 1.2 board

    board.setPrimitiveType(sf::PrimitiveType::Triangles);

    for(Core::Logic::Square sqr = Core::Logic::Square::Start(); sqr.isValid(); ++sqr) 
    {
        const sf::Vector2f cellPos = opt.ToVec(sqr, true);
        const bool isWhite = 1 - (sqr.rank() + sqr.file()) % 2 == 0;

        Utils::AppendQuad(
            cellPos, 
            opt.cell_size(), 
            (
                isWhite 
                ? colorBuilder.Extract<Resources::WHITE_COLOR>()
                : colorBuilder.Extract<Resources::BLACK_COLOR>()      
            ),
            board
        );
    }


    // 2. подписываем координаты

    Resources::Text::Options fileNotationOpt, rankNotationOpt;
    const float characterSize = bopt.padding() * 0.75; 

    // 2.1 file

    fileNotationOpt.characterSize = characterSize;
    fileNotationOpt.pos = sf::Vector2f{
        bopt.left_bottom().x + bopt.padding() + opt.cell_size().x / 2 - characterSize / 2,
        bopt.left_bottom().y - bopt.padding()
    }; 
    std::string_view files = (opt.player().is(Core::Logic::WHITE) ? "ABCDEFGH" : "HGFEDCBA");
    for(char c : files) {
        fileNotationOpt.text = c;
        textBuilder.Push(fileNotationOpt);
        fileNotationOpt.pos.x += opt.cell_size().x;
    }

    // 2.2 rank

    rankNotationOpt.characterSize = characterSize;
    rankNotationOpt.pos = sf::Vector2f{
        bopt.left_bottom().x + bopt.padding() / 2 - characterSize / 4,
        bopt.left_bottom().y - bopt.padding() - opt.cell_size().y / 2 - characterSize / 2
    };
    std::string_view ranks = (opt.player().is(Core::Logic::WHITE) ? "12345678" : "87654321");
    for(char c : ranks) {
        rankNotationOpt.text = c;
        textBuilder.Push(rankNotationOpt);
        rankNotationOpt.pos.y -= opt.cell_size().y;
    }

}

void Board::Render(sf::RenderWindow& window) const
{
    window.draw(background);
    window.draw(board);

    for(const sf::Text& text : textBuilder)
        window.draw(text);

    using namespace Resources;
    sf::VertexArray highlight(sf::PrimitiveType::Triangles);

    if(selected) 
        BuildHighlight<SELECTED_SQUARE_COLOR>(selected.value(), highlight);
    for(const Core::Logic::Square& sqr : valid) 
        BuildHighlight<VALID_SQUARE_COLOR>(sqr, highlight);
    if(hover)
        BuildHighlight<HOVER_SQUARE_COLOR>(hover.value(), highlight);

    window.draw(highlight);
}

template <Resources::ColorType T>
void Board::BuildHighlight(Core::Logic::Square sqr, sf::VertexArray& highlight) const
{   
    Utils::AppendQuad(
        opt.ToVec(sqr, true), 
        opt.cell_size(), 
        colorBuilder.Extract<T>(), 
        highlight
    );
}

}