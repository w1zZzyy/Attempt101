#pragma once 

#include "scene/game/event/game-started.hpp"
#include "ui/renderer/pieces.hpp"
#include "../in-progress.hpp"

#include <optional>

namespace Scene::Game::State::Piece
{

class InProgressIdle : public InProgress<UI::Renderer::Pieces> {
public:

    std::optional<InProgress> HandleEventImpl(const Event::GameStarted& event) {return std::nullopt;}

    template<Model::EventType T>
    constexpr bool SupportsImpl() {
        return std::is_same_v<T, Event::GameStarted>;
    }
    
};

}