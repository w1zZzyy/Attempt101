#include "storage.hpp"

namespace game::logic
{


namespace
{


void stcopy(State& dst, const State& src)
{
    dst.hash = src.hash;
    dst.castle = src.castle;
    dst.rule50 = src.rule50;

    dst.move = {};
    dst.passant = NO_SQUARE;
    dst.captured = NO_PIECE;
}


}


State &StateStorage::create()
{
    int curr = size - 1;

    if(size != 0) {
        stcopy(history[size], history[curr]);
    }
    
    ++size;

    return history[curr + 1];
}

State &StateStorage::rollback()
{
    --size;
    return history[size - 1];
}

State &StateStorage::top()
{
    return history[size - 1];
}

const State &StateStorage::top() const
{
    return history[size - 1];
}

bool StateStorage::repetition() const
{
    int curr = size - 1;
    int counter = 1;

    for(int iter = curr - 2; iter >= 0; iter -= 2) 
    {
        if(history[iter].hash == history[curr].hash) 
        {
            counter++;
            if(counter == 3) 
                return true;
        }
        

        if(history[iter].rule50 != history[curr].rule50 - (curr - iter)) 
            return false;
    }

    return false;
}


}