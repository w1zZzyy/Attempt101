#include "promotion.hpp"
#include "core/logic/defs.hpp"
#include "core/logic/square.hpp"
#include "ui/resources/colors.hpp"
#include "ui/resources/textures.hpp"
#include "ui/utils/quad.hpp"

namespace UI::Renderer
{

Promotion::Promotion(const Options::BoardVisual &opt) : 
    opt(opt), 
    colorBuilder(ASSETS_PATH"promotion/colors.csv")
{}

void Promotion::Init() 
{
    Core::Logic::Piece promo[Total] = {
        Core::Logic::QUEEN, 
        Core::Logic::KNIGHT, 
        Core::Logic::BISHOP, 
        Core::Logic::ROOK
    };

    for(int i = 0; i < Total; ++i) {
        state[i].data.texture = Resources::PieceTextures::Get(promo[i], opt.player());
        state[i].name = promo[i];
    }

    const Core::Logic::Square left_bottom = (opt.player().is(Core::Logic::WHITE) 
        ? Core::Logic::a1
        : Core::Logic::h8
    );

    const sf::Vector2f origin = opt.ToVec(left_bottom, true);
    const sf::Vector2f size = sf::Vector2f{
        opt.cell_size().x * 8,
        opt.cell_size().y * 8
    };
    const sf::Color clr = colorBuilder.Extract<Resources::PROMOTION_BLUR>();

    blur.setPrimitiveType(sf::PrimitiveType::Triangles);
    Utils::AppendQuad(origin, size, clr, blur);
}

void Promotion::Show(Core::Logic::Square on)
{
    show = true;

    const int factor = opt.player().is(Core::Logic::WHITE) ? 1 : -1;
    const float y = opt.cell_size().y;

    sf::Vector2f origin = opt.ToVec(on, true);

    for(int i = 0; i < Total; ++i) 
    {
        state[i].sqr = on;
        UpdateBG<Resources::PROMOTION_FRAME>(i);
        on -= 8 * factor;
        origin.y += y;
    }
}

void Promotion::Hide() noexcept 
{
    show = false;
    hover = Core::Logic::NO_SQUARE;

    for(int i = 0; i < Total; ++i)
        state[i].background.clear();
}

std::optional<Core::Logic::Piece> Promotion::Choose(sf::Vector2f pos) const
{
    if(!show)
        return std::nullopt;
    
    if(auto sqr = opt.ToSquare(pos))
        for(int i = 0; i < Total; ++i)
            if(state[i].sqr == *sqr)
                return state[i].name;
            
    return std::nullopt;
}

void Promotion::Hover(sf::Vector2f pos)
{
    if(!show)
        return;

    Core::Logic::Square newHover;

    if(auto sqr = opt.ToSquare(pos)) {
        for(int i = 0; i < Total; ++i) {
            if(state[i].sqr == *sqr) {
                newHover = *sqr;
                UpdateBG<Resources::PROMOTION_HOVER>(i);     
            }
            else if(state[i].sqr == hover) 
                UpdateBG<Resources::PROMOTION_FRAME>(i);
        }
    }

    hover = newHover;
}

void Promotion::Render(sf::RenderWindow &window) const
{
    if(!show)
        return;

    window.draw(blur);
    
    for(int i = 0; i < Total; ++i) 
        window.draw(state[i].background, state[i].data);
}

template<Resources::ColorType T>
void Promotion::UpdateBG(int stateIndex)
{
    state[stateIndex].background.clear();
    state[stateIndex].background.setPrimitiveType(sf::PrimitiveType::Triangles);
    Utils::AppendQuad(
        opt.ToVec(state[stateIndex].sqr, true),
        opt.cell_size(),
        colorBuilder.Extract<T>(),
        state[stateIndex].background, 
        &state[stateIndex].data
    );
}

}