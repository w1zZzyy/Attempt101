#include "test_movegen.hpp"
#include <strstream>

int main()
{
    game::logic::Position::Init();
    std::string_view tests_path = "tests_data/test_node";
    TestPtr test = std::make_unique<NodesCountTest>();

    for(size_t i = 1; i <= 6; ++i)
    {
        std::ostrstream file_builder;
        file_builder << tests_path << i << ".txt";
        const std::string file_path = file_builder.str();

        test->parse_file(file_path);
        test->run();
    }
    
}