#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "core/logic/defs.hpp"

namespace UI::Resources 
{

class PieceTextures {
public:

    using Ptr = std::shared_ptr<sf::Texture>;
    static const Ptr& Get(Core::Logic::Piece piece, Core::Logic::Color color);

private:

    static Ptr textures[Core::Logic::PIECE_COUNT][Core::Logic::COLOR_COUNT];

};

}