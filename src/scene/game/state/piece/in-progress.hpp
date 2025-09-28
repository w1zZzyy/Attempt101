#pragma once 

#include "scene/model/state.hpp"
#include "ui/renderer/pieces.hpp"
#include "../../event/game-started.hpp"
#include <optional>

namespace Scene::Game::State::Piece
{

class InProgress : public Model::IState<InProgress, UI::Renderer::Pieces> {
public:
    std::optional<InProgress> HandleEventImpl(const Event::GameStarted& event) {return std::nullopt;}
};

}