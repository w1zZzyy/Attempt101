#include "text.hpp"

#include "SFML/Graphics/Text.hpp"

namespace UI::Resources 
{

void Text::Push(const Options& opt)
{
    sf::Text& text = texts.emplace_back(font);

    text.setString(opt.text.c_str());
    text.setCharacterSize(opt.characterSize);
    text.setPosition(opt.pos);
}

}