#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "logic/defs.hpp"

namespace resource
{

class ConfigManager {
public:

    ConfigManager();
    
    sf::Color WhiteSquare() const {return SquareColor("white");}
    sf::Color BlackSquare() const {return SquareColor("black");}
    sf::Color Highlight() const;
    sf::Vector2f SquareSize() const;
    sf::Vector2f LeftBottomSquare() const;

    std::string InitalFen() const;
    game::logic::Color BoardView() const;
    int EngineSearchDepth() const;

    sf::Vector2u WindowSize() const;
    std::string Title() const;
    int FPS() const;

private:

    sf::Color SquareColor(std::string_view clr) const;

private:

    nlohmann::json config;
    
};

}