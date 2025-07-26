#include "Simulator.h"
#include <queue>

void Simulator::handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGateType)
{
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mousePos), view);

            // Check for gate/pin selection
            bool hitGate = false;
            for (size_t i = 0; i < gates.size(); ++i)
            {
                if (gates[i].getType() != GateType::OUTPUT)
                {
                    sf::Vector2f outPin = gates[i].getOutputPinPosition();
                    if (sf::FloatRect(outPin - sf::Vector2f{5.f, 5.f}, {10.f, 10.f}).contains(worldPos))
                    {
                        if (selectingSource && selectedGate == std::numeric_limits<size_t>::max())
                        {
                            selectedGate = i;
                            selectedPin = -1; // Output pin
                            selectingSource = false;
                        }
                        hitGate = true;
                        break;
                    }
                }
                if (gates[i].getType() != GateType::INPUT)
                {
                    int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
                    for (int j = 0; j < inputCount; ++j)
                    {
                        sf::Vector2f inPin = gates[i].getInputPinPosition(j);
                        if (sf::FloatRect(inPin - sf::Vector2f{5.f, 5.f}, {10.f, 10.f}).contains(worldPos))
                        {
                            if (!selectingSource && selectedGate != std::numeric_limits<size_t>::max() && selectedGate < gates.size())
                            {
                                wires.emplace_back(selectedGate, -1, i, j);
                                selectedGate = std::numeric_limits<size_t>::max();
                                selectedPin = -1;
                                selectingSource = true;
                            }
                            hitGate = true;
                            break;
                        }
                    }
                }
                if (gates[i].getType() == GateType::INPUT && gates[i].getBounds().contains(worldPos))
                {
                    gates[i].setState(!gates[i].getState()); // Toggle input
                    hitGate = true;
                    break;
                }
            }

            // Place new gate if no gate/pin was hit and not in wire placement mode
            if (!hitGate && selectedGate == std::numeric_limits<size_t>::max())
            {
                gates.emplace_back(selectedGateType, worldPos); // Use selectedGateType instead of selectedGate
            }
        }
    }
}

void Simulator::addInput(sf::Vector2f position)
{
    gates.emplace_back(GateType::INPUT, position);
}

void Simulator::update()
{
    // Update wire positions
    for (auto &wire : wires)
    {
        if (wire.getSrcGate() < gates.size() && wire.getDstGate() < gates.size())
        {
            sf::Vector2f start = gates[wire.getSrcGate()].getOutputPinPosition();
            sf::Vector2f end = gates[wire.getDstGate()].getInputPinPosition(wire.getDstPin());
            wire.setPositions(start, end);
        }
    }
    evaluateCircuit();
}

void Simulator::evaluateCircuit()
{
    // Reset gate states (except inputs)
    for (auto &gate : gates)
    {
        if (gate.getType() != GateType::INPUT)
        {
            gate.setState(false);
        }
    }

    // Evaluate gates in a simplified manner
    std::vector<bool> evaluated(gates.size(), false);
    for (size_t i = 0; i < gates.size(); ++i)
    {
        if (gates[i].getType() == GateType::INPUT || evaluated[i])
            continue;
        std::vector<bool> inputs;
        int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
        for (int j = 0; j < inputCount; ++j)
        {
            bool inputValue = false;
            for (const auto &wire : wires)
            {
                if (wire.getDstGate() == i && wire.getDstPin() == j && wire.getSrcGate() < gates.size())
                {
                    inputValue = gates[wire.getSrcGate()].getState();
                    break;
                }
            }
            inputs.push_back(inputValue);
        }
        if (inputs.size() == inputCount)
        {
            gates[i].setState(gates[i].evaluate(inputs));
            evaluated[i] = true;
        }
    }
}

void Simulator::draw(sf::RenderWindow &window) const
{
    for (const auto &gate : gates)
    {
        gate.draw(window);
    }
    for (const auto &wire : wires)
    {
        wire.draw(window);
    }
}