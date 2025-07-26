#pragma once
#include <SFML/Graphics.hpp>

enum class GateType
{
    AND,
    OR,
    NOT
};

class Gate
{
public:
    Gate(GateType type, sf::Vector2f position);

    void draw(sf::RenderWindow &window) const;
    bool evaluate(bool input1, bool input2 = false) const;
    sf::FloatRect getBounds() const;

private:
    GateType type;
    sf::Vector2f position;
    sf::RectangleShape shape;
};