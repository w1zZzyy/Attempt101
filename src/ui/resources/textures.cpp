#include "textures.hpp"
#include <format>

namespace UI::Resources
{

std::string BuildPath(Core::Logic::Piece piece, Core::Logic::Color color)
{
    std::string res;

    res += (color == Core::Logic::WHITE) ? 'w' : 'b';

    switch (piece) {
        case Core::Logic::PAWN:   res += 'p';   break;
        case Core::Logic::KNIGHT: res += 'n';   break;
        case Core::Logic::BISHOP: res += 'b';   break;
        case Core::Logic::ROOK:   res += 'r';   break;
        case Core::Logic::QUEEN:  res += 'q';   break;
        case Core::Logic::KING:   res += 'k';   break;
        default: throw std::logic_error("Invalid piece");
    }

    return std::format(ASSETS_PATH "/textures/{}.png", res);
}

const PieceTextures::Ptr& PieceTextures::Get(Core::Logic::Piece piece, Core::Logic::Color color)
{
    assert(piece.isValid() && color.isValid());

    Ptr& texture = textures[piece][color];

    if(!texture) {
        const std::string path = BuildPath(piece, color);
        texture = std::make_shared<sf::Texture>();
        if(!texture->loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);  
        }
        texture->setSmooth(true);
    }

    return texture;
}


}