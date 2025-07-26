#pragma once
#include <SFML/Graphics.hpp>

class Wire
{
public:
    Wire(sf::Vector2f start, sf::Vector2f end);

    void draw(sf::RenderWindow &window) const;

private:
    sf::VertexArray line;
};