#include "Wire.h"

Wire::Wire(size_t srcGate, int srcPin, size_t dstGate, int dstPin)
    : srcGate(srcGate), srcPin(srcPin), dstGate(dstGate), dstPin(dstPin)
{
    line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    line[0].color = sf::Color::Yellow;
    line[1].color = sf::Color::Yellow;
}

void Wire::draw(sf::RenderWindow &window) const
{
    // Draw thicker line by drawing multiple lines
    for (int offset = -1; offset <= 1; ++offset)
    {
        sf::VertexArray thickLine = line;
        thickLine[0].position.y += offset;
        thickLine[1].position.y += offset;
        window.draw(thickLine);
    }
}

void Wire::setPositions(sf::Vector2f start, sf::Vector2f end)
{
    line[0].position = start;
    line[1].position = end;
}

void Wire::setState(bool active)
{
    sf::Color color = active ? sf::Color::Red : sf::Color(100, 100, 100);
    line[0].color = color;
    line[1].color = color;
}

size_t Wire::getSrcGate() const { return srcGate; }
int Wire::getSrcPin() const { return srcPin; }
size_t Wire::getDstGate() const { return dstGate; }
int Wire::getDstPin() const { return dstPin; }