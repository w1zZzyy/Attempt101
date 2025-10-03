#pragma once 

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "../model/options.hpp"

#include <optional>

namespace UI::Renderer 
{

class Pieces 
{
    using Piece = std::optional<sf::Sprite>;
public:

    Pieces(const Options::BoardVisual& opt) noexcept : opt(opt) {}

    void Append(Core::Logic::Color, Core::Logic::Piece, Core::Logic::Square);
    void Move(Core::Logic::Square from, Core::Logic::Square targ);
    void Replace(Core::Logic::Piece newPiece, Core::Logic::Color newColor, Core::Logic::Square on);
    void Render(sf::RenderWindow&) const;

    Piece& GetPiece(Core::Logic::Square on) noexcept {return pieces[on];} 

private:

    void SetSize(sf::Sprite&);
    void SetCenter(sf::Sprite&);

private:

    const Options::BoardVisual& opt;
    Piece pieces[Core::Logic::SQUARE_COUNT];

};

}