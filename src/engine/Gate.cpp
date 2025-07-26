#include "Gate.h"

Gate::Gate(GateType type, sf::Vector2f position) : type(type), position(position)
{
    shape.setSize({50.f, 50.f});
    shape.setPosition(position);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);

    switch (type)
    {
    case GateType::INPUT:
        shape.setFillColor(sf::Color(128, 128, 128)); // Gray for low/off
        break;
    case GateType::OUTPUT:
        shape.setFillColor(sf::Color(128, 128, 128)); // Gray for low/off
        break;
    case GateType::AND:
        shape.setFillColor(sf::Color(200, 200, 200)); // Light gray for logic gates
        break;
    case GateType::OR:
        shape.setFillColor(sf::Color(200, 200, 200)); // Light gray for logic gates
        break;
    case GateType::NOT:
        shape.setFillColor(sf::Color(200, 200, 200)); // Light gray for logic gates
        break;
    }
}

void Gate::draw(sf::RenderWindow &window) const
{
    // Draw gate body with selection highlight
    sf::RectangleShape gateShape = shape;
    if (selected)
    {
        gateShape.setOutlineThickness(4.f);
        gateShape.setOutlineColor(sf::Color::Yellow); // Make selection more visible
    }
    window.draw(gateShape);

    // Draw gate label at center
    drawGateLabel(window);

    // Draw pins
    sf::CircleShape pin(6.f);
    pin.setOutlineThickness(1.f);
    pin.setOutlineColor(sf::Color::Black);

    // Output pin
    if (type != GateType::OUTPUT)
    {
        pin.setFillColor(state ? sf::Color::Red : sf::Color::White);
        pin.setPosition(getOutputPinPosition() - sf::Vector2f{6.f, 6.f});
        window.draw(pin);
    }

    // Input pins
    if (type != GateType::INPUT)
    {
        pin.setFillColor(sf::Color::White);
        for (int i = 0; i < (type == GateType::NOT ? 1 : 2); ++i)
        {
            pin.setPosition(getInputPinPosition(i) - sf::Vector2f{6.f, 6.f});
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
        shape.setFillColor(state ? sf::Color::Red : sf::Color(128, 128, 128)); // Red for high, gray for low
    }
    else if (type == GateType::OUTPUT)
    {
        shape.setFillColor(state ? sf::Color::Red : sf::Color(128, 128, 128)); // Red for high, gray for low
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

std::string Gate::getGateTypeString() const
{
    switch (type)
    {
    case GateType::INPUT:
        return "IN";
    case GateType::OUTPUT:
        return "OUT";
    case GateType::AND:
        return "AND";
    case GateType::OR:
        return "OR";
    case GateType::NOT:
        return "NOT";
    default:
        return "?";
    }
}

void Gate::drawGateLabel(sf::RenderWindow &window) const
{
    std::string label = getGateTypeString();
    sf::Vector2f center = position + sf::Vector2f{25.f, 25.f}; // Center of 50x50 gate

    // Draw readable text using pixel patterns
    float charWidth = 6.f;
    float charHeight = 7.f;
    float startX = center.x - (label.length() * charWidth) / 2.f;
    float startY = center.y - charHeight / 2.f;

    for (size_t i = 0; i < label.length(); ++i)
    {
        drawCharacterPixels(window, label[i], startX + i * charWidth, startY);
    }
}

void Gate::drawCharacterPixels(sf::RenderWindow &window, char c, float x, float y) const
{
    // 5x7 pixel patterns for each character
    std::vector<std::vector<int>> pattern;

    switch (c)
    {
    case 'I':
        pattern = {
            {1, 1, 1},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {1, 1, 1}};
        break;
    case 'N':
        pattern = {
            {1, 0, 0, 1},
            {1, 1, 0, 1},
            {1, 0, 1, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1}};
        break;
    case 'O':
        pattern = {
            {0, 1, 1, 0},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 1, 1, 0}};
        break;
    case 'U':
        pattern = {
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 1, 1, 0}};
        break;
    case 'T':
        pattern = {
            {1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0}};
        break;
    case 'A':
        pattern = {
            {0, 1, 1, 0},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 1, 1, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1}};
        break;
    case 'D':
        pattern = {
            {1, 1, 1, 0},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 1, 1, 0}};
        break;
    case 'R':
        pattern = {
            {1, 1, 1, 0},
            {1, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 1, 1, 0},
            {1, 1, 0, 0},
            {1, 0, 1, 0},
            {1, 0, 0, 1}};
        break;
    default:
        // Empty pattern for unknown characters
        pattern = {{0}};
        break;
    }

    // Draw the pixels
    for (size_t row = 0; row < pattern.size(); ++row)
    {
        for (size_t col = 0; col < pattern[row].size(); ++col)
        {
            if (pattern[row][col] == 1)
            {
                sf::RectangleShape pixel({1.f, 1.f});
                pixel.setPosition({x + col, y + row});
                pixel.setFillColor(sf::Color::Black);
                window.draw(pixel);
            }
        }
    }
}
void Gate::setSelected(bool isSelected)
{
    selected = isSelected;
}

bool Gate::isSelected() const
{
    return selected;
}
