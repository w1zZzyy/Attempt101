#include "idle.hpp"

#include "in-progress.hpp"

namespace Scene::Game::State::Promotion 
{

Model::NextState<InProgressIdle> Idle::HandleEventImpl(const Event::Promotion &event)
{
    object->Show(event.on);
    
    Model::NextState<InProgressIdle> next;
    next.Load<Object>(std::move(
        InProgressIdle()
        .SetFrom(event.from)
        .SetTarg(event.on)
    ));
    return next;
}

}