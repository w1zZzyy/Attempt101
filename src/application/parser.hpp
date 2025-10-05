#pragma once 

#include "logic/defs.hpp"

#include <optional>
#include <unordered_map>
#include <string>

class Parser {
public:

    Parser(int argc, char* argv[]);

    std::optional<Core::Logic::Color> player() const;
    std::optional<std::string> fen() const;
    std::optional<uint16_t> time_limit() const;
    std::optional<uint8_t> max_depth() const;
    std::optional<uint32_t> tt_size() const;
    std::optional<std::string> log() const;

private:

    const std::string* find(std::string command) const;

private:

    std::unordered_map<std::string, std::string> commands;

};