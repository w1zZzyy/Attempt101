#include "board.hpp"

#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../resources/colors.hpp"
#include "../utils/quad.hpp"
#include "../utils/cellsize.hpp"

#include <string_view>

namespace UI::Renderer 
{

Board::Board() : 
    textBuilder(ASSETS_PATH "/board/font.ttf"),
    colorBuilder(ASSETS_PATH "/board/colors.csv") 
{}

void Board::Init(const Model::Options& opt)
{
    // 1. инициализируем статичные вершины

    background.setPrimitiveType(sf::PrimitiveType::Triangles);
    Utils::AppendQuad(
        opt.origin, opt.size, 
        colorBuilder.Extract<Resources::BACKGROUND_COLOR>(), 
        background
    );

    board.setPrimitiveType(sf::PrimitiveType::Triangles);

    const sf::Vector2f boardOrigin = sf::Vector2f(
        opt.origin.x + opt.padding, 
        opt.origin.y - opt.padding
    );
    const sf::Vector2f cellSize = Utils::computeCellSize(opt.size, opt.padding);

    for(Core::Logic::Square sqr = Core::Logic::Square::Start(); sqr.isValid(); ++sqr) 
    {
        int rank = sqr.rank();
        int file = sqr.file();

        const sf::Vector2f cellPos = sf::Vector2f(
            boardOrigin.x + cellSize.x * file, 
            boardOrigin.y - cellSize.y * rank
        );

        const bool isWhite = 1 - (rank + file) % 2 == 0;

        Utils::AppendQuad(
            cellPos, cellSize, 
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
    const float characterSize = opt.padding * 0.75; 

    // 2.1 file

    fileNotationOpt.characterSize = characterSize;
    fileNotationOpt.pos = sf::Vector2f{
        opt.origin.x + opt.padding + cellSize.x / 2 - characterSize / 2,
        opt.origin.y - opt.padding
    }; 
    std::string_view files = (opt.player.is(Core::Logic::WHITE) ? "ABCDEFGH" : "HGFEDCBA");
    for(char c : files) {
        fileNotationOpt.text = c;
        textBuilder.Push(fileNotationOpt);
        fileNotationOpt.pos.x += cellSize.x;
    }

    // 2.2 rank

    rankNotationOpt.characterSize = characterSize;
    rankNotationOpt.pos = sf::Vector2f{
        opt.origin.x + opt.padding / 2 - characterSize / 4,
        opt.origin.y - opt.padding - cellSize.y / 2 - characterSize / 2
    };
    std::string_view ranks = (opt.player.is(Core::Logic::WHITE) ? "12345678" : "87654321");
    for(char c : ranks) {
        rankNotationOpt.text = c;
        textBuilder.Push(rankNotationOpt);
        rankNotationOpt.pos.y -= cellSize.y;
    }

}

void Board::Render(sf::RenderWindow& window) const
{
    window.draw(background);
    window.draw(board);

    for(const sf::Text& text : textBuilder)
        window.draw(text);
}

}