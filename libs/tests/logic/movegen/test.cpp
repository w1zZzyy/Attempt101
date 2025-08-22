#include "test.hpp"
#include "logic/movelist.hpp"
#include "logic/position.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

using namespace game::logic;

size_t CountNodes(TPosition& pos, size_t depth)
{
    if(depth == 0) {
        return 1;
    }

    pos.compute_enemy_attackers().compute_pins_from_sliders();

    MoveList moves;
    moves.generate(pos);

    size_t nodes = 0;

    for(size_t i = 0; i < moves.get_size(); ++i)
    {
        pos.do_move(moves[i]);
        nodes += CountNodes(pos, depth - 1);
        pos.undo_move();
    }

    return nodes;
}

void ITest::parse_file(const std::string& file_path)
{
    std::ifstream file(file_path);
    parse_file(file);
}

void ITest::parse_file(std::ifstream& file)
{
    std::getline(file, fen);

    std::string line;
    std::getline(file, line);

    depth = std::stoll(line);
}

void NodesCountTest::parse_file(std::ifstream& file) 
{
    ITest::parse_file(file);

    std::string line;
    std::getline(file, line);

    expected_nodes = std::stoll(line);
}

void NodesCountTest::run() 
{
    TPosition pos(fen);

    auto start = std::chrono::high_resolution_clock::now();
    size_t actual_nodes = CountNodes(pos, depth);
    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration<double>(end - start).count();

    std::cout << "Time: " << time << "\n";
    std::cout << "Actual: " << actual_nodes << "\tExpected: " << expected_nodes << "\n";
    std::cout << (actual_nodes == expected_nodes ? "Passed" : "Failed");
    std::cout << "\n--------------------------------------------------\n";
}

void BranchesCountTest::parse_file(std::ifstream& file) 
{
    ITest::parse_file(file);

    std::string line;
    while(std::getline(file, line))
    {
        std::stringstream ss_line(line);

        std::string move, num;
        std::getline(ss_line, move, ':');
        std::getline(ss_line, num);
        
        data.insert({move, std::stoll(num)});
    }
}

void BranchesCountTest::run() 
{
    TPosition pos(fen);

    MoveList moves;
    moves.generate(pos);

    std::vector<std::string> bad_fens;

    for(size_t i = 0; i < moves.get_size(); ++i)
    {
        if(moves[i].from() == d1 && moves[i].targ() == a1) {
            std::cout << "";
        }

        pos.do_move(moves[i]);

        size_t nodes = CountNodes(pos, depth - 1);
        if(!correct(moves[i], nodes)) {
            bad_fens.push_back(pos.fen());
        }

        pos.undo_move();
    }

    if(!bad_fens.empty()) {
        std::cout << "Failed\nWrong Fens:\n";
        for(std::string& fen : bad_fens) {
            std::cout << fen << "\n";
        }
    } else {
        std::cout << "Passed\n";
    }

    std::cout << "--------------------------------------------------\n";
}

bool BranchesCountTest::correct(game::logic::Move move, size_t nodes) const
{
    std::ostringstream os;
    os << move;
    
    auto iter = data.find(os.str());
    if(iter == data.end()) {
        std::cout << "Branch: " << move << " Not founded\n";
        return false;
    }

    if(iter->second != nodes) {
        std::cout << "Branch: " << move << " is Wrong\t";
        std::cout << "Expected: " << iter->second << "\tActual: " << nodes << "\n";
        return false;
    }

    return true;
}
