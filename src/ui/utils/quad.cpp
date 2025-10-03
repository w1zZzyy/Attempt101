#include "quad.hpp"

namespace UI::Utils 
{

void AppendQuad(sf::Vector2f origin, sf::Vector2f size, sf::Color clr, sf::VertexArray& arr)
{
    sf::Vertex lb, rb, lt, rt;

    lb.color = rb.color = lt.color = rt.color = clr;
    lb.position = origin;
    rb.position = {origin.x + size.x, origin.y};
    lt.position = {origin.x, origin.y - size.y};
    rt.position = {origin.x + size.x, origin.y - size.y};

    arr.append(lb);
    arr.append(rb);
    arr.append(lt);

    arr.append(rt);
    arr.append(rb);
    arr.append(lt);
}

}