#pragma once 

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "ui/model/options.hpp"
#include "ui/resources/colors.hpp"

namespace UI::Renderer
{

class Promotion 
{
    static constexpr int Total = 4;
public:

    Promotion(const Options::BoardVisual&);

    void Init();

    void Show(Core::Logic::Square on);
    void Hide() noexcept;

    std::optional<Core::Logic::Piece> Choose(sf::Vector2f pos) const;
    void Hover(sf::Vector2f pos);

    void Render(sf::RenderWindow&) const;

private:

    template<Resources::ColorType T>
    void UpdateBG(int stateIndex);

private:

    const Options::BoardVisual& opt;
    sf::VertexArray blur;
    Resources::Colors colorBuilder;
    struct State {
        sf::RenderStates data;
        Core::Logic::Piece name;
        Core::Logic::Square sqr;
        sf::VertexArray background;
    } state[Total];
    Core::Logic::Square hover = Core::Logic::NO_SQUARE;
    bool show = false;

};

}