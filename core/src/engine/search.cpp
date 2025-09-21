#include "search.hpp"

namespace Core::Engine
{

Search::Search()
{
    static bool init = false;
    if(!init) {
        Evaluation::Setup();
        init = true;
    }
}

}