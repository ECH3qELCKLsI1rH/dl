#include "Wire.h"

Wire::Wire(sf::Vector2f start, sf::Vector2f end)
{
    line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    line[0].position = start;
    line[1].position = end;
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
}

void Wire::draw(sf::RenderWindow &window) const
{
    window.draw(line);
}