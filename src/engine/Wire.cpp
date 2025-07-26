#include "Wire.h"

Wire::Wire(size_t srcGate, int srcPin, size_t dstGate, int dstPin)
    : srcGate(srcGate), srcPin(srcPin), dstGate(dstGate), dstPin(dstPin)
{
    line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
}

void Wire::draw(sf::RenderWindow &window) const
{
    window.draw(line);
}

void Wire::setPositions(sf::Vector2f start, sf::Vector2f end)
{
    line[0].position = start;
    line[1].position = end;
}

size_t Wire::getSrcGate() const { return srcGate; }
int Wire::getSrcPin() const { return srcPin; }
size_t Wire::getDstGate() const { return dstGate; }
int Wire::getDstPin() const { return dstPin; }