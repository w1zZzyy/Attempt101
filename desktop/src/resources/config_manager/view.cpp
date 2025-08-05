#include "view.hpp"

#include <format>
#include <fstream>
#include <iostream>

namespace resource
{


ConfigManager::ConfigManager(std::string_view config_name)
{
    std::string path = ASSETS_PATH + std::format(
        "/configs/{}.json",
        config_name
    );

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "file not found\n";
        return;
    }

    file >> config;
}

sf::Vector2f BoardConfigManager::SquareSize() const
{
    return sf::Vector2f{
        config["square_size"]["width"], 
        config["square_size"]["height"]
    };
}

sf::Vector2f BoardConfigManager::LeftBottomSquare() const
{
    return sf::Vector2f{
        config["left_bottom_square"]["x"],
        config["left_bottom_square"]["y"]
    };
}

std::string BoardConfigManager::InitalFen() const
{
    return config["initial_fen"];
}

sf::Color BoardConfigManager::SquareColor(std::string_view clr) const
{
    const auto& sqr = config["square_color"];
    return sf::Color{
        sqr[clr]["red"], 
        sqr[clr]["green"],
        sqr[clr]["blue"]
    };
}

sf::Vector2u WindowConfigManager::WindowSize() const
{
    return sf::Vector2u{
        config["width"], 
        config["height"]
    };
}

std::string WindowConfigManager::Title() const
{
    return config["title"];
}

int WindowConfigManager::FPS() const
{
    return config["fps"];
}

}