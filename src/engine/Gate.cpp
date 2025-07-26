#include "Gate.h"

Gate::Gate(GateType type, sf::Vector2f position) : type(type), position(position)
{
    shape.setSize({50.f, 50.f});
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void Gate::draw(sf::RenderWindow &window) const
{
    window.draw(shape);
}

bool Gate::evaluate(bool input1, bool input2) const
{
    switch (type)
    {
    case GateType::AND:
        return input1 && input2;
    case GateType::OR:
        return input1 || input2;
    case GateType::NOT:
        return !input1;
    default:
        return false;
    }
}

sf::FloatRect Gate::getBounds() const
{
    return shape.getGlobalBounds();
}