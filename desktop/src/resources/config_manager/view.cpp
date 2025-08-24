#include "view.hpp"

#include <iostream>
#include <fstream>

namespace resource
{


ConfigManager::ConfigManager()
{
    std::string path = ASSETS_PATH + std::string{"/configs/static_config.json"};

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "file not found\n";
        return;
    }

    file >> config;
}

sf::Color ConfigManager::Highlight() const
{
    const auto& hl = config["board"]["highlight"];
    return sf::Color{
        hl["red"], hl["green"], hl["blue"], 
        hl["alpha"]
    };
}

sf::Vector2f ConfigManager::SquareSize() const
{
    const auto& sqr = config["board"]["square_size"];
    return sf::Vector2f{
        sqr["width"], 
        sqr["height"]
    };
}

sf::Vector2f ConfigManager::LeftBottomSquare() const
{
    const auto& sqr = config["board"]["left_bottom_square"];
    return sf::Vector2f{
        sqr["x"],
        sqr["y"]
    };
}

std::string ConfigManager::InitalFen() const
{
    return config["game"]["start_position"];
}

game::logic::Color ConfigManager::BoardView() const
{
    using namespace game::logic;

    const auto& plr = config["game"]["player_side"];

    if(plr == "white") return WHITE;
    if(plr == "black") return BLACK;

    throw std::runtime_error("invalid player_side field");
}

int ConfigManager::EngineSearchDepth() const {
    return config["game"]["engine"]["search_depth"];
}

size_t ConfigManager::EngineTranspositionSize() const {
    return config["game"]["engine"]["transpositions_mb"];
}

sf::Color ConfigManager::SquareColor(std::string_view clr) const
{
    const auto& sqr = config["board"]["square_color"];
    return sf::Color{
        sqr[clr]["red"], 
        sqr[clr]["green"],
        sqr[clr]["blue"]
    };
}

sf::Vector2u ConfigManager::WindowSize() const
{
    const auto& win = config["window"];
    return sf::Vector2u{
        win["width"], 
        win["height"]
    };
}

std::string ConfigManager::Title() const {
    return config["window"]["title"];
}

int ConfigManager::FPS() const {
    return config["window"]["fps"];
}

}