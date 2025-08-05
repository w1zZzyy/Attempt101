#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

namespace resource
{


class ConfigManager {
public:
    ConfigManager(std::string_view config_name);
protected:
    nlohmann::json config;
};


class BoardConfigManager : public ConfigManager {
public:
    BoardConfigManager() : ConfigManager("board_config") {}
    sf::Color WhiteSquare() const {return SquareColor("white");}
    sf::Color BlackSquare() const {return SquareColor("black");}
    sf::Vector2f SquareSize() const;
    sf::Vector2f LeftBottomSquare() const;
    std::string InitalFen() const;
private:
    sf::Color SquareColor(std::string_view clr) const;
};


class WindowConfigManager : public ConfigManager {
public:
    WindowConfigManager() : ConfigManager("window_config") {}
    sf::Vector2u WindowSize() const;
    std::string Title() const;
    int FPS() const;
};


}