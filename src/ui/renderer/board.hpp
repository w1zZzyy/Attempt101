#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include "../resources/text.hpp"
#include "../resources/colors.hpp"
#include "../model/options.hpp"
#include "core/logic/square.hpp"

namespace UI::Renderer 
{

class Board
{
public:

    Board(const Options::BoardVisual& opt);

    void Init(const Options::Board&);
    void Render(sf::RenderWindow&) const;

    void SetSelected(const std::optional<Core::Logic::Square>& sqr) noexcept {selected = sqr;}
    void SetHover(const std::optional<Core::Logic::Square>& sqr) noexcept {hover = sqr;}
    void AppendValid(Core::Logic::Square sqr) noexcept {valid.push_back(sqr);}

    void RemoveSelected() noexcept {selected.reset();}
    void RemoveValid() noexcept {valid.clear();}
    void RemoveHover() noexcept {hover.reset();}

    std::optional<Core::Logic::Square> GetSelected() const noexcept {return selected;}
    const std::vector<Core::Logic::Square>& GetValid() const noexcept {return valid;}

private:

    template<Resources::ColorType T>   
    void BuildHighlight(Core::Logic::Square, sf::VertexArray&) const;

private:

    const Options::BoardVisual& opt;

    sf::VertexArray background, board;
    Resources::Text textBuilder;
    Resources::Colors colorBuilder;

    std::optional<Core::Logic::Square> selected;
    std::vector<Core::Logic::Square> valid;
    std::optional<Core::Logic::Square> hover;

};

}