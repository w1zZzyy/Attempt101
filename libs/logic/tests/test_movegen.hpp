#pragma once

#include <cstddef>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <string>

#include "../src/position.hpp"
#include "../src/move.hpp"

size_t CountNodes(PositionFixedMemory& pos, size_t depth);


class ITest
{
public:
    void parse_file(const std::string& file_path);
    virtual void run() = 0;
    virtual ~ITest() = default;
protected:
    virtual void parse_file(std::ifstream& file);

    std::string fen;
    size_t depth;

};

class NodesCountTest : public ITest
{
public:
    void run() override;
private:
    void parse_file(std::ifstream& file) override;

    size_t expected_nodes;

};

class BranchesCountTest : public ITest
{
public:
    void run() override;
private:
    void parse_file(std::ifstream& file) override;
    bool correct(game::logic::Move move, size_t nodes) const;

    std::unordered_map<std::string, size_t> data; 
};

using TestPtr = std::unique_ptr<ITest>;