#include "quad.hpp"

#include "SFML/Graphics/Texture.hpp"

namespace UI::Utils 
{

void AppendQuad(
    sf::Vector2f origin, 
    sf::Vector2f size, 
    sf::Color clr, 
    sf::VertexArray& arr, 
    sf::RenderStates* st
) {
    sf::Vertex lb, rb, lt, rt;

    lb.color = rb.color = lt.color = rt.color = clr;

    lb.position = origin;
    rb.position = {origin.x + size.x, origin.y};
    lt.position = {origin.x, origin.y - size.y};
    rt.position = {origin.x + size.x, origin.y - size.y};

    if(st) 
    {
        sf::Vector2u size = st->texture->getSize();

        lb.texCoords = {0, (float)size.y};
        lt.texCoords = {0, 0};
        rb.texCoords = {(float)size.x, (float)size.y};
        rt.texCoords = {(float)size.x, 0};
    }

    arr.append(lb);
    arr.append(rb);
    arr.append(lt);

    arr.append(rt);
    arr.append(rb);
    arr.append(lt);
}

}