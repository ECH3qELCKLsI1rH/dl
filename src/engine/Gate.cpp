#include "Gate.h"

Gate::Gate(GateType type, sf::Vector2f position) : type(type), position(position)
{
    shape.setSize({50.f, 50.f});
    shape.setPosition(position);
    switch (type)
    {
    case GateType::INPUT:
        shape.setFillColor(sf::Color::Blue);
        break;
    case GateType::OUTPUT:
        shape.setFillColor(sf::Color::Red);
        break;
    default:
        shape.setFillColor(sf::Color::Green);
        break;
    }
}

void Gate::draw(sf::RenderWindow &window) const
{
    window.draw(shape);
    // Draw pins
    sf::CircleShape pin(5.f);
    pin.setFillColor(sf::Color::White);
    if (type != GateType::OUTPUT)
    {
        pin.setPosition(getOutputPinPosition() - sf::Vector2f{5.f, 5.f});
        window.draw(pin);
    }
    if (type != GateType::INPUT)
    {
        for (int i = 0; i < (type == GateType::NOT ? 1 : 2); ++i)
        {
            pin.setPosition(getInputPinPosition(i) - sf::Vector2f{5.f, 5.f});
            window.draw(pin);
        }
    }
}

bool Gate::evaluate(const std::vector<bool> &inputs) const
{
    switch (type)
    {
    case GateType::AND:
        return inputs.size() >= 2 && inputs[0] && inputs[1];
    case GateType::OR:
        return inputs.size() >= 2 && (inputs[0] || inputs[1]);
    case GateType::NOT:
        return inputs.size() >= 1 && !inputs[0];
    case GateType::INPUT:
        return state;
    case GateType::OUTPUT:
        return inputs.size() >= 1 ? inputs[0] : false;
    default:
        return false;
    }
}

sf::FloatRect Gate::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Gate::getInputPinPosition(int index) const
{
    if (type == GateType::NOT)
    {
        return position + sf::Vector2f{0.f, 25.f};
    }
    return position + sf::Vector2f{0.f, index == 0 ? 15.f : 35.f};
}

sf::Vector2f Gate::getOutputPinPosition() const
{
    return position + sf::Vector2f{50.f, 25.f};
}

void Gate::setState(bool state)
{
    this->state = state;
    if (type == GateType::INPUT)
    {
        shape.setFillColor(state ? sf::Color::Cyan : sf::Color::Blue);
    }
    else if (type == GateType::OUTPUT)
    {
        shape.setFillColor(state ? sf::Color::Magenta : sf::Color::Red);
    }
}

bool Gate::getState() const
{
    return state;
}

GateType Gate::getType() const
{
    return type;
}