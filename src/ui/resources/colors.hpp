#pragma once

#include "SFML/Graphics/Color.hpp"
#include <optional>
#include <string>
#include <unordered_map>

namespace UI::Resources 
{

class Colors 
{
public:

    Colors(std::string path);
    std::optional<sf::Color> Extract(const std::string& name) const;

private:

    std::unordered_map<std::string, sf::Color> colors;

};

}