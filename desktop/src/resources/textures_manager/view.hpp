#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "logic/src/defs.hpp"

namespace resource
{


class TextureManager
{
public:

    using TexturePtr = std::shared_ptr<sf::Texture>;
    static TexturePtr Get(game::logic::Color c, game::logic::Piece p);

private:

    struct TextureLoad {
        TexturePtr texture = std::make_shared<sf::Texture>();
        bool is_loaded = false;
    };

private:

    static TextureLoad textures[game::logic::COLOR_COUNT][game::logic::PIECE_COUNT];

};


}