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
    VALID_SQUARE_COLOR,
    SELECTED_SQUARE_COLOR,
    HOVER_SQUARE_COLOR, 
    PROMOTION_HOVER,
    PROMOTION_BLUR,
    PROMOTION_FRAME
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