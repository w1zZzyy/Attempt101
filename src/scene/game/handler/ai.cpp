#include "ai.hpp"

#include "scene/game/event/game-started.hpp"
#include "scene/game/event/game-updates.hpp"

namespace Scene::Game::Handler 
{

void AIHandler::Init(const Core::Engine::Search::Options &options)
{
    options.onMove = [this](Core::Engine::Search::Info info){
        bus.Publish<Event::GameUpdateAttempted>({info.bestMove});
    };

    engine.Init(options);

    OnGame<Event::GameStarted>();
    OnGame<Event::GameUpdated>();
}

template<Model::EventType TEvent>
void AIHandler::OnGame()
{
    constexpr bool InitPos = std::is_same_v<TEvent, Event::GameStarted>;

    bus.Subscribe<TEvent>({
        [this](const TEvent& event) 
        {
            if constexpr (InitPos) {
                auto& c_event = static_cast<const Event::GameStarted&>(event); 
                pos = &c_event.pos;
                engine.SetPosition(c_event.pos);
                player = c_event.player.opp();
            }
            
            if(pos->GetSide() == player) 
                engine.Think();
        }
    });
}

}