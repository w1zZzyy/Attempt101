#pragma once

#include "SFML/Graphics/Color.hpp"

#include <string>
#include <unordered_map>

namespace UI::Resources 
{

enum ColorType {
    WHITE_COLOR,
    BLACK_COLOR,
    BACKGROUND_COLOR,
    DANGER_SQUARE_COLOR,
    VALID_SQUARE_COLOR,
    SELECTED_SQUARE_COLOR
};

class Colors 
{
public:

    Colors(std::string path);

    template<ColorType T>
    sf::Color Extract() const;

private:

    std::unordered_map<std::string, sf::Color> colors;

};

}