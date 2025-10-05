#include "parser.hpp"

#include <exception>
#include <iostream>

Parser::Parser(int argc, char* argv[])
{
    for(int i = 1; i < argc; ++i)
    {
        std::string_view cmd = argv[i];

        if(cmd.size() < 2)
                continue;

        if(cmd.starts_with("--") && i < argc - 1) {
            i++;
            std::string_view data = argv[i];
            commands.insert({cmd.data(), data.data()});
        }
    }
}

const std::string* Parser::find(std::string command) const
{
    if(auto it = commands.find( "--" + command); it != commands.end())
        return &it->second;
    return nullptr;
}

std::optional<Core::Logic::Color> Parser::player() const
{
    if(const std::string* clr = find("player")) {
        if(*clr == "white") return Core::Logic::WHITE;
        if(*clr == "black") return Core::Logic::BLACK;
    }
    return std::nullopt;
}

std::optional<std::string> Parser::fen() const
{
    if(const std::string* fen = find("fen")) {
        return *fen;
    }
    return std::nullopt;
}


std::optional<uint16_t> Parser::time_limit() const
{
    if(const std::string* limit = find("time-limit")) {
        try {return std::stoi(*limit);}
        catch(const std::exception& e) {std::cerr << e.what();}
    }
    return std::nullopt;
}

std::optional<uint8_t> Parser::max_depth() const
{
    if(const std::string* depth = find("max-depth")) {
        try {return std::stoi(*depth);}
        catch(const std::exception& e) {std::cerr << e.what();}
    }
    return std::nullopt;
}

std::optional<uint32_t> Parser::tt_size() const
{
    if(const std::string* size = find("tt-size-mb")) {
        try {return std::stoi(*size);}
        catch(const std::exception& e) {std::cerr << e.what();}
    }
    return std::nullopt;
}

std::optional<std::string> Parser::log() const
{
    if(const std::string* log = find("log")) {
        return *log;
    }
    return std::nullopt;
}
