#include "in-progress.hpp"

#include "idle.hpp"
#include "scene/game/event/game-updates.hpp"

namespace Scene::Game::State::Promotion 
{

InProgressIdle &InProgressIdle::SetFrom(Core::Logic::Square from) noexcept
{
    this->from = from;
    return *this;
}

InProgressIdle &InProgressIdle::SetTarg(Core::Logic::Square targ) noexcept
{
    this->targ = targ;
    return *this;
}

Model::NoNextState<InProgressIdle::Object> InProgressIdle::HandleEventImpl(const Event::MouseMoved &event)
{
    object->Hover(event.pos);
    return {};
}

Model::NextState<Idle> InProgressIdle::HandleEventImpl(const Event::MousePressed& event)
{
    if(std::optional piece = object->Choose(event.pos)) 
    {
        using namespace Core::Logic;

        auto publish = [this](MoveFlag flag) {
            bus->Publish<Event::GameUpdateAttempted>(Move{from, targ, flag});
        };

        switch (piece->type())
        {
        case QUEEN:
            publish(Q_PROMOTION_MF);
            break;
        case KNIGHT:
            publish(K_PROMOTION_MF);
            break;
        case BISHOP:
            publish(B_PROMOTION_MF);
            break;
        case ROOK:
            publish(R_PROMOTION_MF);
            break;
        default:
            break;
        }
    }

    object->Hide();

    Model::NextState<Idle> next;
    next.Load<Object>(Idle{});
    return next;
}

}