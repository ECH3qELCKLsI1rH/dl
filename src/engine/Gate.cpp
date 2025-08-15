#include "Gate.h"

#include <numeric>  // for std::accumulate

Gate::Gate(GateType type, sf::Vector2f position) : type(type), position(position) {
    shape.setSize({50.f, 50.f});
    shape.setPosition(position);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);

    switch (type) {
        case GateType::INPUT:
        case GateType::OUTPUT:
            shape.setFillColor(sf::Color(128, 128, 128));
            break;
        case GateType::AND:
        case GateType::OR:
        case GateType::NOT:
            shape.setFillColor(sf::Color(200, 200, 200));
            break;
    }
}

void Gate::draw(sf::RenderWindow &window) const {
    sf::RectangleShape gateShape = shape;
    if (selected) {
        gateShape.setOutlineThickness(4.f);
        gateShape.setOutlineColor(sf::Color::Yellow);
    }
    window.draw(gateShape);

    drawGateLabel(window);

    sf::CircleShape pin(6.f);
    pin.setOutlineThickness(1.f);
    pin.setOutlineColor(sf::Color::Black);

    // Output pin
    if (type != GateType::OUTPUT) {
        pin.setFillColor(state ? sf::Color::Red : sf::Color::White);
        pin.setPosition(getOutputPinPosition() - sf::Vector2f{6.f, 6.f});
        window.draw(pin);
    }

    // Input pins
    int inputCount = getInputCount();
    pin.setFillColor(sf::Color::White);
    for (int i = 0; i < inputCount; ++i) {
        pin.setPosition(getInputPinPosition(i) - sf::Vector2f{6.f, 6.f});
        window.draw(pin);
    }
}

int Gate::getInputCount() const {
    if (type == GateType::NOT || type == GateType::OUTPUT) return 1;
    return std::max(1, static_cast<int>(inputGates.size()));
}

bool Gate::evaluate() const {
    std::vector<bool> inputStates;
    for (auto *g : inputGates) {
        inputStates.push_back(g->getState());
    }
    return evaluate(inputStates);
}

bool Gate::evaluate(const std::vector<bool> &inputs) const {
    switch (type) {
        case GateType::AND:
            return !inputs.empty() && std::all_of(inputs.begin(), inputs.end(), [](bool v) { return v; });
        case GateType::OR:
            return std::any_of(inputs.begin(), inputs.end(), [](bool v) { return v; });
        case GateType::NOT:
            return inputs.empty() ? false : !inputs[0];
        case GateType::INPUT:
            return state;
        case GateType::OUTPUT:
            return inputs.empty() ? false : inputs[0];
        case GateType::NAND:
            return inputs.empty() ? true : !std::all_of(inputs.begin(), inputs.end(), [](bool v) { return v; });
        case GateType::NOR:
            return !std::any_of(inputs.begin(), inputs.end(), [](bool v) { return v; });
        case GateType::XOR:
            return inputs.size() == 2 ? (inputs[0] != inputs[1]) : false;
        default:
            return false;
    }
}

sf::Vector2f Gate::getInputPinPosition(int pinIndex) const {
    int count = getInputCount();
    if (count == 1) return position + sf::Vector2f{0.f, shape.getSize().y / 2.f};

    float spacing = shape.getSize().y / (count + 1);
    return position + sf::Vector2f{0.f, spacing * (pinIndex + 1)};
}

sf::Vector2f Gate::getOutputPinPosition() const { return position + sf::Vector2f{shape.getSize().x, shape.getSize().y / 2.f}; }

void Gate::drawGateLabel(sf::RenderWindow &window) const {
    if (!font) return;  // safety check

    sf::Text text(*font);
    text.setString(getGateTypeString());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    text.setPosition(position + shape.getSize() / 2.f);
    window.draw(text);
}
