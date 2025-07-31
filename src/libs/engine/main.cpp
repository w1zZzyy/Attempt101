#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>

#include "search.hpp"

bool isnum(std::string_view str)
{
    return !str.empty() &&
        std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return std::isdigit(c);
        }
    );
}

int main()
{
    game::logic::Position::Init();
    game::engine::Eval::Setup();

    game::engine::Search search;

    while(true)
    {
        std::cout << "input fen: ";
        std::string fen;
        std::getline(std::cin, fen);

        std::cout << "input depth: ";
        std::string depth;
        std::getline(std::cin, depth);

        if(!isnum(depth)) {
            std::cerr << "depth is not num\n";
            continue;
        }

        game::engine::SearchOptions options{fen, std::stoi(depth)};
        game::engine::SearchResults res = search.start(options);

        std::cout << "best move: " << res.move << "\nscore: " << res.score << "\n";
    }
}