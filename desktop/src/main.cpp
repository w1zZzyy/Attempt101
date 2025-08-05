#include "application/view.hpp"

int main() 
{
    game::logic::Position::Init();

    Application app;
    app.run();

    return 0;
}