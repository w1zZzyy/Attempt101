#include "test_movegen.hpp"

#include <format>
#include <string>

int main()
{
    PositionFixedMemory::Init();
    TestPtr test = std::make_unique<NodesCountTest>();

    for(size_t i = 1; i <= 6; ++i)
    {
        const std::string file_path = std::format(
            "{}/test_node{}{}",
            LOGIC_TESTS_DATA_PATH, i, ".txt" 
        );

        test->parse_file(file_path);
        test->run();
    }
    
}