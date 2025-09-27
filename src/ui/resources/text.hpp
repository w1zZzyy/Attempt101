#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"

#include <string>
#include <vector>

namespace UI::Resources 
{

class Text {
public:

    struct Options {
        std::string text;
        unsigned int characterSize;
        sf::Vector2f pos;
    };

    Text(std::string font_path) : font(font_path) {}

    void Push(const Options&);
    const sf::Font& Font() const noexcept {return font;}
    
    const std::vector<sf::Text>::const_iterator begin() const { return texts.begin(); }
    const std::vector<sf::Text>::const_iterator end() const { return texts.end(); }

private:

    sf::Font font;
    std::vector<sf::Text> texts;

};

}