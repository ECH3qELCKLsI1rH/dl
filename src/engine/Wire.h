#pragma once
#include <SFML/Graphics.hpp>

class Wire
{
public:
    Wire(size_t srcGate, int srcPin, size_t dstGate, int dstPin);

    void draw(sf::RenderWindow &window) const;
    size_t getSrcGate() const;
    int getSrcPin() const;
    size_t getDstGate() const;
    int getDstPin() const;
    void setPositions(sf::Vector2f start, sf::Vector2f end);
    void setState(bool active);

private:
    sf::VertexArray line;
    size_t srcGate;
    int srcPin;
    size_t dstGate;
    int dstPin;
};