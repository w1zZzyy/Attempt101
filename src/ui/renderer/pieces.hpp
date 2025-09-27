#pragma once 

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"

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

    void Init(const Model::Options&);
    void Append(Core::Logic::Color, Core::Logic::Piece, Core::Logic::Square);
    void Move(Core::Logic::Square from, Core::Logic::Square targ);
    void Replace(Core::Logic::Piece newPiece, Core::Logic::Color newColor, Core::Logic::Square on);
    void Render(sf::RenderWindow&) const;

private:

    sf::Vector2f ToVec(Core::Logic::Square) const;
    void SetSize(sf::Sprite&);
    void SetCenter(sf::Sprite&);

private:

    Core::Logic::Color player;
    sf::Vector2f cellSize;
    sf::Vector2f origin;
    
    Piece pieces[Core::Logic::SQUARE_COUNT];

};

}