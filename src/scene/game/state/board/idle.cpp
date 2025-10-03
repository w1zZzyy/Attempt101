#include "idle.hpp"

#include "in-progress.hpp"
#include "ui/renderer/board.hpp"

namespace Scene::Game::State::Board 
{

using Object = UI::Renderer::Board;

Model::NextState<InProgressIdle> Idle::HandleEventImpl(const Event::GameStarted& event) 
{
    Model::NextState<InProgressIdle> next;
    next.Load<Object>(InProgressIdle{event.player, event.pos, event.moves});
    return next;
}

}