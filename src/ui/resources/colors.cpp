#include "colors.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>

namespace  
{

sf::Color parseHex(const std::string& hex)
{
    if(hex.size() != 6) {
        throw std::runtime_error("hex size must be equal to 6");
    }

    std::string sub1 = hex.substr(0, 2);
    std::string sub2 = hex.substr(2, 2);
    std::string sub3 = hex.substr(4, 2);

    int r = std::stoi(sub1, nullptr, 16);
    int g = std::stoi(sub2, nullptr, 16);
    int b = std::stoi(sub3, nullptr, 16);

    return {
        uint8_t(r),
        uint8_t(g),
        uint8_t(b)
    };
}

}

namespace UI::Resources 
{

Colors::Colors(std::string path)
{
    std::ifstream file(ASSETS_PATH "board/colors.csv");
    if(!file) {
        throw std::runtime_error("Failed to open colors file");
    }

    std::string line;
    while(std::getline(file, line)) 
    {
        size_t delimiterPos = line.find('#');
        if(delimiterPos == std::string::npos) {
            throw std::runtime_error("Invalid line format in colors file: " + line);
        }

        std::string name(line.substr(0, delimiterPos));
        std::string value(line.substr(delimiterPos + 1));

        colors.insert({name, parseHex(value)});
    }
}

std::optional<sf::Color> Colors::Extract(const std::string& name) const
{
    if(auto it = colors.find(name); it != colors.end()) {
        return it->second;
    }
    return std::nullopt;
}


}