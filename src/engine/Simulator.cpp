#include "Simulator.h"
#include <queue>
#include <iostream>
#include <sstream>
#include <algorithm>

void Simulator::handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGateType)
{
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};

            // Check for close button clicks in UI elements (these use screen coordinates)
            if (showExpression)
            {
                sf::FloatRect closeBtn({1110.f, 55.f}, {60.f, 20.f}); // Expression close button
                if (closeBtn.contains(mousePos))
                {
                    showExpression = false;
                    return;
                }
            }

            if (showTruthTable)
            {
                sf::FloatRect closeBtn({1110.f, 205.f}, {60.f, 20.f}); // Truth table close button
                if (closeBtn.contains(mousePos))
                {
                    showTruthTable = false;
                    return;
                }
            }

            // Skip if click is in the palette area
            if (mousePos.x <= 120.f)
                return;

            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mousePos), view);

            // Check for gate/pin selection
            bool hitGate = false;
            for (size_t i = 0; i < gates.size(); ++i)
            {
                // Check output pins
                if (gates[i].getType() != GateType::OUTPUT)
                {
                    sf::Vector2f outPin = gates[i].getOutputPinPosition();
                    if (sf::FloatRect(outPin - sf::Vector2f{8.f, 8.f}, {16.f, 16.f}).contains(worldPos))
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

                // Check input pins
                if (gates[i].getType() != GateType::INPUT)
                {
                    int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
                    for (int j = 0; j < inputCount; ++j)
                    {
                        sf::Vector2f inPin = gates[i].getInputPinPosition(j);
                        if (sf::FloatRect(inPin - sf::Vector2f{8.f, 8.f}, {16.f, 16.f}).contains(worldPos))
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

                // Check gate body for selection or input toggle
                if (gates[i].getBounds().contains(worldPos))
                {
                    if (gates[i].getType() == GateType::INPUT)
                    {
                        gates[i].setState(!gates[i].getState()); // Toggle input
                    }
                    selectGateAt(worldPos);
                    hitGate = true;
                    break;
                }
            }

            // Place new gate if no gate/pin was hit and not in wire placement mode
            if (!hitGate && selectedGate == std::numeric_limits<size_t>::max())
            {
                // Create gate with unique naming for inputs/outputs
                if (selectedGateType == GateType::INPUT)
                {
                    gates.emplace_back(selectedGateType, worldPos);
                    inputCounter++;
                }
                else if (selectedGateType == GateType::OUTPUT)
                {
                    gates.emplace_back(selectedGateType, worldPos);
                    outputCounter++;
                }
                else
                {
                    gates.emplace_back(selectedGateType, worldPos);
                }
            }
        }
        else if (clicked->button == sf::Mouse::Button::Right)
        {
            // Cancel wire placement or deselect all gates
            cancelSelection();
        }
    }
}

void Simulator::addInput(sf::Vector2f position)
{
    gates.emplace_back(GateType::INPUT, position);
}

void Simulator::clearCircuit()
{
    gates.clear();
    wires.clear();
    selectedGate = std::numeric_limits<size_t>::max();
    selectedPin = -1;
    selectingSource = true;
    selectedGates.clear();
    currentExpression.clear();
    truthTable.clear();
    showTruthTable = false;
    showExpression = false;
    inputCounter = 0;
    outputCounter = 0;
    std::cout << "Circuit cleared!" << std::endl;
}

void Simulator::selectGateAt(sf::Vector2f worldPos)
{
    // Clear previous selections
    for (auto &gate : gates)
    {
        gate.setSelected(false);
    }
    selectedGates.clear();

    // Select gate at position
    for (size_t i = 0; i < gates.size(); ++i)
    {
        if (gates[i].getBounds().contains(worldPos))
        {
            gates[i].setSelected(true);
            selectedGates.push_back(i);
            break;
        }
    }
}
void Simulator::deleteSelectedGates()
{
    if (selectedGates.empty())
        return;

    // Sort indices in descending order to avoid index shifting issues
    std::sort(selectedGates.begin(), selectedGates.end(), std::greater<size_t>());

    for (size_t gateIndex : selectedGates)
    {
        if (gateIndex < gates.size())
        {
            removeGate(gateIndex);
        }
    }

    selectedGates.clear();
    std::cout << "Deleted selected gates" << std::endl;
}

void Simulator::cancelSelection()
{
    // Cancel wire placement
    selectedGate = std::numeric_limits<size_t>::max();
    selectedPin = -1;
    selectingSource = true;

    // Clear gate selections
    for (auto &gate : gates)
    {
        gate.setSelected(false);
    }
    selectedGates.clear();
}

void Simulator::removeGate(size_t gateIndex)
{
    if (gateIndex >= gates.size())
        return;

    // Remove all wires connected to this gate
    removeWiresConnectedToGate(gateIndex);

    // Remove the gate
    gates.erase(gates.begin() + gateIndex);

    // Update wire indices (shift down by 1 for gates with index > gateIndex)
    for (auto &wire : wires)
    {
        if (wire.getSrcGate() > gateIndex)
        {
            // Need to update wire source gate index - this requires Wire modification
        }
        if (wire.getDstGate() > gateIndex)
        {
            // Need to update wire destination gate index - this requires Wire modification
        }
    }
}

void Simulator::removeWiresConnectedToGate(size_t gateIndex)
{
    wires.erase(
        std::remove_if(wires.begin(), wires.end(),
                       [gateIndex](const Wire &wire)
                       {
                           return wire.getSrcGate() == gateIndex || wire.getDstGate() == gateIndex;
                       }),
        wires.end());
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
    // Reset all gate states except inputs
    for (auto &gate : gates)
    {
        if (gate.getType() != GateType::INPUT)
        {
            gate.setState(false);
        }
    }

    // Topological evaluation
    std::vector<bool> evaluated(gates.size(), false);
    bool progress = true;

    while (progress)
    {
        progress = false;
        for (size_t i = 0; i < gates.size(); ++i)
        {
            if (evaluated[i] || gates[i].getType() == GateType::INPUT)
            {
                if (gates[i].getType() == GateType::INPUT)
                    evaluated[i] = true;
                continue;
            }

            // Check if all inputs are available
            std::vector<bool> inputs;
            int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
            bool allInputsReady = true;

            for (int j = 0; j < inputCount; ++j)
            {
                bool inputValue = false;
                bool inputFound = false;

                for (const auto &wire : wires)
                {
                    if (wire.getDstGate() == i && wire.getDstPin() == j && wire.getSrcGate() < gates.size())
                    {
                        if (evaluated[wire.getSrcGate()])
                        {
                            inputValue = gates[wire.getSrcGate()].getState();
                            inputFound = true;
                            break;
                        }
                        else
                        {
                            allInputsReady = false;
                            break;
                        }
                    }
                }

                if (!allInputsReady)
                    break;
                inputs.push_back(inputValue);
            }

            if (allInputsReady && inputs.size() == inputCount)
            {
                gates[i].setState(gates[i].evaluate(inputs));
                evaluated[i] = true;
                progress = true;
            }
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

    // Draw selection indicators
    if (selectedGate < gates.size())
    {
        sf::CircleShape indicator(12.f);
        indicator.setFillColor(sf::Color::Transparent);
        indicator.setOutlineThickness(2.f);
        indicator.setOutlineColor(sf::Color::Cyan);
        sf::Vector2f pos = gates[selectedGate].getOutputPinPosition();
        indicator.setPosition(pos - sf::Vector2f{12.f, 12.f});
        window.draw(indicator);
    }
}

void Simulator::drawUI(sf::RenderWindow &window) const
{
    sf::View originalView = window.getView();
    sf::View uiView({600.f, 400.f}, {1200.f, 800.f});
    window.setView(uiView);

    float rightSide = 800.f;

    if (showExpression && !currentExpression.empty())
    {
        sf::RectangleShape expressionBg({380.f, 120.f});
        expressionBg.setPosition({rightSide, 50.f});
        expressionBg.setFillColor(sf::Color(0, 0, 0, 180));
        expressionBg.setOutlineThickness(2.f);
        expressionBg.setOutlineColor(sf::Color::White);
        window.draw(expressionBg);

        // Draw title
        drawSimpleText(window, "Logical Expression:", sf::Vector2f(rightSide + 10.f, 60.f), sf::Color::White);

        // Draw expression text
        drawSimpleText(window, currentExpression, sf::Vector2f(rightSide + 10.f, 80.f), sf::Color::Yellow);

        // Draw close button
        sf::RectangleShape closeBtn({60.f, 20.f});
        closeBtn.setPosition({rightSide + 310.f, 55.f});
        closeBtn.setFillColor(sf::Color::Red);
        closeBtn.setOutlineThickness(1.f);
        closeBtn.setOutlineColor(sf::Color::White);
        window.draw(closeBtn);
        drawSimpleText(window, "CLOSE", sf::Vector2f(rightSide + 320.f, 60.f), sf::Color::White);

        // Instructions
        drawSimpleText(window, "Press E again or click CLOSE to hide", sf::Vector2f(rightSide + 10.f, 140.f), sf::Color(128, 128, 128));
    }

    if (showTruthTable && !truthTable.empty())
    {
        float tableHeight = std::min(400.f, (float)truthTable.size() * 20.f + 80.f);
        sf::RectangleShape tableBg({380.f, tableHeight});
        tableBg.setPosition({rightSide, 200.f});
        tableBg.setFillColor(sf::Color(0, 0, 0, 180));
        tableBg.setOutlineThickness(2.f);
        tableBg.setOutlineColor(sf::Color::White);
        window.draw(tableBg);

        // Draw title
        drawSimpleText(window, "Truth Table:", sf::Vector2f(rightSide + 10.f, 210.f), sf::Color::White);

        // Draw close button
        sf::RectangleShape closeBtn({60.f, 20.f});
        closeBtn.setPosition({rightSide + 310.f, 205.f});
        closeBtn.setFillColor(sf::Color::Red);
        closeBtn.setOutlineThickness(1.f);
        closeBtn.setOutlineColor(sf::Color::White);
        window.draw(closeBtn);
        drawSimpleText(window, "CLOSE", sf::Vector2f(rightSide + 320.f, 210.f), sf::Color::White);

        // Draw table headers and data
        float yPos = 230.f;
        for (size_t i = 0; i < std::min((size_t)15, truthTable.size()); ++i)
        {
            drawSimpleText(window, truthTable[i], sf::Vector2f(rightSide + 10.f, yPos), sf::Color::White);
            yPos += 18.f;
        }

        if (truthTable.size() > 15)
        {
            drawSimpleText(window, "... (showing first 15 rows)", sf::Vector2f(rightSide + 10.f, yPos), sf::Color(128, 128, 128));
        }

        // Instructions
        drawSimpleText(window, "Press T again or click CLOSE to hide", sf::Vector2f(rightSide + 10.f, 200.f + tableHeight - 20.f), sf::Color(128, 128, 128));
    }

    window.setView(originalView);
}

std::vector<size_t> Simulator::getInputGates() const
{
    std::vector<size_t> inputs;
    for (size_t i = 0; i < gates.size(); ++i)
    {
        if (gates[i].getType() == GateType::INPUT)
        {
            inputs.push_back(i);
        }
    }
    return inputs;
}

std::vector<size_t> Simulator::getOutputGates() const
{
    std::vector<size_t> outputs;
    for (size_t i = 0; i < gates.size(); ++i)
    {
        if (gates[i].getType() == GateType::OUTPUT)
        {
            outputs.push_back(i);
        }
    }
    return outputs;
}

void Simulator::generateLogicalExpression()
{
    // Toggle if already showing
    if (showExpression)
    {
        showExpression = false;
        return;
    }

    auto outputs = getOutputGates();
    if (outputs.empty())
    {
        currentExpression = "No OUTPUT gates found";
        showExpression = true;
        std::cout << currentExpression << std::endl;
        return;
    }

    std::map<size_t, std::string> expressions;
    auto inputs = getInputGates();

    // Assign variable names to inputs
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        char varName = 'A' + i;
        expressions[inputs[i]] = std::string(1, varName);
    }

    // Generate expression for the first output
    currentExpression = generateExpressionForGate(outputs[0], expressions);
    showExpression = true;

    std::cout << "Generated expression: " << currentExpression << std::endl;
}

std::string Simulator::generateExpressionForGate(size_t gateIndex, std::map<size_t, std::string> &expressions) const
{
    if (expressions.find(gateIndex) != expressions.end())
    {
        return expressions[gateIndex];
    }

    if (gateIndex >= gates.size())
    {
        return "?";
    }

    const Gate &gate = gates[gateIndex];
    std::string result;

    switch (gate.getType())
    {
    case GateType::INPUT:
        result = "INPUT";
        break;

    case GateType::NOT:
    {
        // Find input
        for (const auto &wire : wires)
        {
            if (wire.getDstGate() == gateIndex && wire.getDstPin() == 0)
            {
                std::string input = generateExpressionForGate(wire.getSrcGate(), expressions);
                result = "¬(" + input + ")";
                break;
            }
        }
        if (result.empty())
            result = "¬(?)";
        break;
    }

    case GateType::AND:
    {
        std::vector<std::string> inputs;
        for (int pin = 0; pin < 2; ++pin)
        {
            for (const auto &wire : wires)
            {
                if (wire.getDstGate() == gateIndex && wire.getDstPin() == pin)
                {
                    inputs.push_back(generateExpressionForGate(wire.getSrcGate(), expressions));
                    break;
                }
            }
        }
        if (inputs.size() >= 2)
            result = "(" + inputs[0] + " ∧ " + inputs[1] + ")";
        else
            result = "(? ∧ ?)";
        break;
    }

    case GateType::OR:
    {
        std::vector<std::string> inputs;
        for (int pin = 0; pin < 2; ++pin)
        {
            for (const auto &wire : wires)
            {
                if (wire.getDstGate() == gateIndex && wire.getDstPin() == pin)
                {
                    inputs.push_back(generateExpressionForGate(wire.getSrcGate(), expressions));
                    break;
                }
            }
        }
        if (inputs.size() >= 2)
            result = "(" + inputs[0] + " ∨ " + inputs[1] + ")";
        else
            result = "(? ∨ ?)";
        break;
    }

    case GateType::OUTPUT:
    {
        // Find input to output gate
        for (const auto &wire : wires)
        {
            if (wire.getDstGate() == gateIndex && wire.getDstPin() == 0)
            {
                result = generateExpressionForGate(wire.getSrcGate(), expressions);
                break;
            }
        }
        if (result.empty())
            result = "?";
        break;
    }
    }

    expressions[gateIndex] = result;
    return result;
}

void Simulator::generateTruthTable()
{
    // Toggle if already showing
    if (showTruthTable)
    {
        showTruthTable = false;
        return;
    }

    auto inputs = getInputGates();
    auto outputs = getOutputGates();

    if (inputs.empty() || outputs.empty())
    {
        truthTable = {"No inputs or outputs found"};
        showTruthTable = true;
        std::cout << "Cannot generate truth table: No inputs or outputs found" << std::endl;
        return;
    }

    truthTable.clear();

    // Header with unique input/output names
    std::string header;
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        header += "In" + std::to_string(i) + "  ";
    }
    header += " | ";
    for (size_t i = 0; i < outputs.size(); ++i)
    {
        header += "Out" + std::to_string(i) + " ";
    }
    truthTable.push_back(header);

    // Separator
    std::string separator(header.length(), '-');
    truthTable.push_back(separator);

    // Generate all combinations
    int numInputs = inputs.size();
    int combinations = 1 << numInputs; // 2^numInputs

    for (int combo = 0; combo < combinations; ++combo)
    {
        // Set input states
        for (int i = 0; i < numInputs; ++i)
        {
            bool value = (combo >> i) & 1;
            gates[inputs[i]].setState(value);
        }

        // Evaluate circuit
        const_cast<Simulator *>(this)->evaluateCircuit();

        // Build row
        std::string row;
        for (int i = 0; i < numInputs; ++i)
        {
            row += (gates[inputs[i]].getState() ? " 1" : " 0");
            row += "   ";
        }
        row += " | ";
        for (size_t i = 0; i < outputs.size(); ++i)
        {
            row += (gates[outputs[i]].getState() ? "  1" : "  0");
            row += "   ";
        }
        truthTable.push_back(row);
    }

    showTruthTable = true;

    std::cout << "Generated truth table with " << combinations << " rows" << std::endl;
}

void Simulator::drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color) const
{
    // Draw simple pixel-based text using rectangles
    for (size_t i = 0; i < text.length(); ++i)
    {
        if (text[i] != ' ')
        {
            sf::RectangleShape charRect({5.f, 7.f});
            charRect.setPosition({position.x + i * 6.f, position.y});
            charRect.setFillColor(color);
            charRect.setOutlineThickness(0.5f);
            charRect.setOutlineColor(sf::Color::Black);
            window.draw(charRect);
        }
    }
}