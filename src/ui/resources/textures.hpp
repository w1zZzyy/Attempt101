#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "core/logic/defs.hpp"

namespace UI::Resources 
{

class PieceTextures {
public:

    using Ptr = const sf::Texture*;
    static Ptr Get(Core::Logic::Piece piece, Core::Logic::Color color);

private:

    struct Texture {
        sf::Texture data;
        bool init = false;
    };
    static Texture textures[Core::Logic::PIECE_COUNT][Core::Logic::COLOR_COUNT];

};

}