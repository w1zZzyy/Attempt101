#include "view.hpp"

#include <format>
#include <iostream>

namespace resource
{

TextureManager::TextureLoad TextureManager::textures[game::logic::COLOR_COUNT][game::logic::PIECE_COUNT];

std::string ColorToString(game::logic::Color c)
{
    switch(c)
    {
    case game::logic::WHITE: return "White";
    case game::logic::BLACK: return "Black";
    default: break;
    }
    return {};
}

std::string PieceToString(game::logic::Piece p)
{
    switch (p)
    {
    case game::logic::KING: return "King";
    case game::logic::QUEEN: return "Queen";
    case game::logic::PAWN: return "Pawn";
    case game::logic::KNIGHT: return "Knight";
    case game::logic::BISHOP: return "Bishop";
    case game::logic::ROOK: return "Rook";
    default: break;
    }
    return {};
}

std::string BuildPath(game::logic::Color c, game::logic::Piece p)
{
    return ASSETS_PATH + std::format(
        "/images/{}{}.png",
        ColorToString(c), PieceToString(p)
    );
}

TextureManager::TexturePtr TextureManager::Get(game::logic::Color c, game::logic::Piece p)
{
    assert(c.isValid() && p.isValid());

    if(textures[c][p].is_loaded) 
        return textures[c][p].texture;
    
    std::string path = BuildPath(c, p);

    if(!textures[c][p].texture->loadFromFile(path)) {
        std::cerr << "cant load from file\n";
        return nullptr;
    }

    textures[c][p].is_loaded = true;
    textures[c][p].texture->setSmooth(true);

    return textures[c][p].texture;
}

}