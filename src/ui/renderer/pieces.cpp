#include "pieces.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../resources/textures.hpp"

#include <cassert>
#include <stdexcept>

namespace UI::Renderer 
{

void Pieces::Append(Core::Logic::Color color, Core::Logic::Piece piece, Core::Logic::Square square)
{
    if(!piece.isValid() || !color.isValid() || !square.isValid())
        return;

    const auto& texture = Resources::PieceTextures::Get(piece, color);
    assert(texture);

    sf::Sprite& sprite = pieces[square].emplace(*texture);

    sprite.setPosition(opt.ToVec(square));
    SetSize(sprite);
    SetCenter(sprite);
}

void Pieces::Move(Core::Logic::Square from, Core::Logic::Square targ)
{
    if(!pieces[from])
        throw std::runtime_error(std::format("no piece on sqr '{}'", from.to_string()));

    pieces[targ] = std::move(pieces[from]);
    pieces[targ]->setPosition(opt.ToVec(targ));

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

void Pieces::SetSize(sf::Sprite& sprite)
{
    const sf::Vector2u currSize = sprite.getTexture().getSize();
    const sf::Vector2f cellSize = opt.cell_size();

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