#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class GateType
{
    AND,
    OR,
    NOT,
    INPUT,
    OUTPUT
};

class Gate
{
public:
    Gate(GateType type, sf::Vector2f position);

    void draw(sf::RenderWindow &window) const;
    bool evaluate(const std::vector<bool> &inputs) const;
    sf::FloatRect getBounds() const;

    sf::Vector2f getInputPinPosition(int index) const;
    sf::Vector2f getOutputPinPosition() const;
    void setState(bool state);
    bool getState() const;
    GateType getType() const;

private:
    GateType type;
    sf::Vector2f position;
    sf::RectangleShape shape;
    bool state = false; // For INPUT & OUTPUT gates
};