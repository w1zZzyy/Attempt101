#include "colors.hpp"
#include "SFML/Graphics/Color.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace UI::Resources 
{

namespace
{

struct RGB {
    std::string name;
    uint32_t r, g, b, a;
};

std::ifstream& operator >> (std::ifstream& in, RGB& rgb) {
    char comma;
    in >> rgb.name >> 
        rgb.r >> comma >> 
        rgb.g >> comma >> 
        rgb.b >> comma >> 
        rgb.a;
    return in;
}

}

Colors::Colors(std::string path)
{
    std::ifstream file(ASSETS_PATH "board/colors.csv");
    if(!file) {
        throw std::runtime_error("Failed to open colors file");
    }

    {
        std::string line;
        std::getline(file, line);
    }

    RGB rgb;
    while(file >> rgb) {
        sf::Color color(rgb.r, rgb.g, rgb.b, rgb.a);
        colors.insert({rgb.name, color});
    }
}

template<ColorType T>
sf::Color Colors::Extract() const
{
    constexpr std::string_view name = (
        T == WHITE_COLOR ? "white" : 
        T == BLACK_COLOR ? "black" : 
        T == BACKGROUND_COLOR ? "background" :
        T == DANGER_SQUARE_COLOR ? "danger" :
        T == VALID_SQUARE_COLOR ? "valid" :
        T == SELECTED_SQUARE_COLOR ? "selected" :
        T == HOVER_SQUARE_COLOR ? "hover" : 
        ""
    );
    static_assert(!name.empty(), "invalid type");

    if(
        auto it = colors.find(name.data()); 
        it != colors.end()
    ) {
        return it->second;
    }

    throw std::runtime_error(std::format(
        "no color '{}' was found",
        name
    ));
}

template sf::Color Colors::Extract<WHITE_COLOR>() const;
template sf::Color Colors::Extract<BLACK_COLOR>() const;
template sf::Color Colors::Extract<BACKGROUND_COLOR>() const;
template sf::Color Colors::Extract<DANGER_SQUARE_COLOR>() const;
template sf::Color Colors::Extract<VALID_SQUARE_COLOR>() const;
template sf::Color Colors::Extract<SELECTED_SQUARE_COLOR>() const;
template sf::Color Colors::Extract<HOVER_SQUARE_COLOR>() const;

}