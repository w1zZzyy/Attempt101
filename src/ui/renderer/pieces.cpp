#include "pieces.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../utils/cellsize.hpp"
#include "../resources/textures.hpp"

#include <cassert>
#include <stdexcept>

namespace UI::Renderer 
{

void Pieces::Init(const Model::Options& opt)
{
    cellSize = Utils::computeCellSize(opt.size, opt.padding);
    origin = sf::Vector2f(
        opt.origin.x + opt.padding + cellSize.x * 0.5,
        opt.origin.y - opt.padding - cellSize.y * 0.5
    );
    player = opt.player;
}

void Pieces::Append(Core::Logic::Color color, Core::Logic::Piece piece, Core::Logic::Square square)
{
    if(!piece.isValid() || !color.isValid() || !square.isValid())
        return;

    const auto& texture = Resources::PieceTextures::Get(piece, color);
    assert(texture);

    sf::Sprite& sprite = pieces[square].emplace(*texture);

    sprite.setPosition(ToVec(square));
    SetSize(sprite);
    SetCenter(sprite);
}

void Pieces::Move(Core::Logic::Square from, Core::Logic::Square targ)
{
    if(!pieces[from])
        throw std::runtime_error(std::format("no piece on sqr '{}'", from.to_string()));

    pieces[targ] = std::move(pieces[from]);
    pieces[targ]->setPosition(ToVec(targ));

    pieces[from].reset();
}

void Pieces::Replace(Core::Logic::Piece newPiece, Core::Logic::Color newColor, Core::Logic::Square on)
{
    if(pieces[on])
        pieces[on].reset();

    const auto& texture = Resources::PieceTextures::Get(newPiece, newColor);
    assert(texture);

    pieces[on].emplace(*texture);
}

void Pieces::Render(sf::RenderWindow& window) const
{
    for(const Piece& piece : pieces) 
        if(piece) 
            window.draw(piece.value());
}

sf::Vector2f Pieces::ToVec(Core::Logic::Square sqr) const
{
    int rank = sqr.rank();
    int file = sqr.file();

    if(player.is(Core::Logic::BLACK)) {
        rank = 7 - rank;
        file = 7 - file;
    }

    return sf::Vector2f{
        origin.x + cellSize.x * file,
        origin.y - cellSize.y * rank,
    };
}

void Pieces::SetSize(sf::Sprite& sprite)
{
    sf::Vector2u currSize = sprite.getTexture().getSize();
    sprite.setScale({
        cellSize.x / currSize.x,
        cellSize.y / currSize.y
    });
}

void Pieces::SetCenter(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.getCenter());
}


}