#include <fstream>
#include <format>

#include "logic/movelist.hpp"
#include "gtest/gtest.h"

using namespace Core::Logic;

using TPosition = Position<StaticStorage>;

struct NodesCounter {size_t expected, actual;};

std::string BuildPath(size_t number) {
    return std::format(
        "{}/test_node{}{}",
        LOGIC_TESTS_DATA_PATH, number, ".txt" 
    );
}

size_t CountNodes(TPosition& pos, size_t depth)
{
    if(depth == 0) {
        return 1;
    }

    pos.UpdateAttacks();

    MoveList moves;
    moves.generate<MoveGenType::All>(pos);

    size_t nodes = 0;

    for(size_t i = 0; i < moves.get_size(); ++i)
    {
        pos.DoMove(moves[i]);
        nodes += CountNodes(pos, depth - 1);
        pos.UndoMove();
    }

    return nodes;
}

NodesCounter RunNodeCounter(const std::string& file_path) 
{
    std::string fen;
    size_t depth;
    size_t expected;

    std::ifstream file(file_path);
    std::getline(file, fen);

    std::string line;

    std::getline(file, line);
    depth = std::stoll(line);

    std::getline(file, line);
    expected = std::stoll(line);

    TPosition pos(fen);
    return {expected, CountNodes(pos, depth)};
}

class NodeCounterParamTest : public ::testing::TestWithParam<int> {};

TEST_P(NodeCounterParamTest, CountsCorrectly) {
    int index = GetParam();
    NodesCounter nc = RunNodeCounter(BuildPath(index));
    EXPECT_EQ(nc.actual, nc.expected) << std::format(
        "\nACTUAL: {} ---- EXPECTED: {}\n", 
        nc.actual, nc.expected
    );
}

INSTANTIATE_TEST_SUITE_P(
    NodeCounterTests,
    NodeCounterParamTest,
    ::testing::Values(1, 2, 3, 4, 5, 6)
);