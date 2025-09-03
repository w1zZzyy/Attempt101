#include "application/view.hpp"

#include "logic/position.hpp"
#include "engine/eval.hpp"

int main() 
{
    PositionDynamicMemory::Init();
    game::engine::Eval::Setup();

    Application app;
    app.run();

    return 0;
}