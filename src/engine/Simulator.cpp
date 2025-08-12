#include "Simulator.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>

void Simulator::handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGateType) {
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (clicked->button == sf::Mouse::Button::Left) {
            // Convert mouse position to window coordinates
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos{(float)mousePixel.x, (float)mousePixel.y};

            // Check for close button clicks in UI elements (these use screen coordinates)
            if (showExpression) {
                sf::FloatRect closeBtn({1110.f, 55.f}, {60.f, 20.f});  // Expression close button
                if (closeBtn.contains(mousePos)) {
                    showExpression = false;
                    return;
                }
            }

            if (showTruthTable) {
                sf::FloatRect closeBtn({1110.f, 205.f}, {60.f, 20.f});  // Truth table close button
                if (closeBtn.contains(mousePos)) {
                    showTruthTable = false;
                    return;
                }
            }

            if (showInputField) {
                sf::FloatRect closeBtn({1110.f, 355.f}, {60.f, 20.f});  // Input field close button
                if (closeBtn.contains(mousePos)) {
                    showInputField = false;
                    inputExpression.clear();
                    return;
                }
            }

            // Skip if click is in the palette area
            if (mousePos.x <= 120.f) return;

            sf::Vector2f worldPos = window.mapPixelToCoords(mousePixel, view);

            // Check for gate/pin selection
            bool hitGate = false;
            for (size_t i = 0; i < gates.size(); ++i) {
                // Check output pins
                if (gates[i].getType() != GateType::OUTPUT) {
                    sf::Vector2f outPin = gates[i].getOutputPinPosition();
                    if (sf::FloatRect(outPin - sf::Vector2f{8.f, 8.f}, {16.f, 16.f}).contains(worldPos)) {
                        if (selectingSource && selectedGate == std::numeric_limits<size_t>::max()) {
                            selectedGate = i;
                            selectedPin = -1;  // Output pin
                            selectingSource = false;
                        }
                        hitGate = true;
                        break;
                    }
                }

                // Check input pins
                if (gates[i].getType() != GateType::INPUT) {
                    int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
                    for (int j = 0; j < inputCount; ++j) {
                        sf::Vector2f inPin = gates[i].getInputPinPosition(j);
                        if (sf::FloatRect(inPin - sf::Vector2f{8.f, 8.f}, {16.f, 16.f}).contains(worldPos)) {
                            if (!selectingSource && selectedGate != std::numeric_limits<size_t>::max() && selectedGate < gates.size()) {
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
                if (gates[i].getBounds().contains(worldPos)) {
                    if (gates[i].getType() == GateType::INPUT) {
                        gates[i].setState(!gates[i].getState());  // Toggle input
                    }
                    selectGateAt(worldPos);
                    hitGate = true;
                    break;
                }
            }

            // Place new gate if no gate/pin was hit and not in wire placement mode
            if (!hitGate && selectedGate == std::numeric_limits<size_t>::max()) {
                // Create gate with unique naming for inputs/outputs
                if (selectedGateType == GateType::INPUT) {
                    gates.emplace_back(selectedGateType, worldPos);
                    inputCounter++;
                } else if (selectedGateType == GateType::OUTPUT) {
                    gates.emplace_back(selectedGateType, worldPos);
                    outputCounter++;
                } else {
                    gates.emplace_back(selectedGateType, worldPos);
                }
            }
        } else if (clicked->button == sf::Mouse::Button::Right) {
            // Cancel wire placement or deselect all gates
            cancelSelection();
        }
    }
    // Handle text input for expression field
    if (showInputField) {
        if (const auto *textEntered = event.getIf<sf::Event::TextEntered>()) {
            if (textEntered->unicode < 128)  // ASCII only
            {
                char c = static_cast<char>(textEntered->unicode);
                // Ignore 'i' or 'I' to prevent it from being added to input
                if (std::tolower(c) == 'i') return;
                if (c == '\b' && !inputExpression.empty())  // Backspace
                {
                    inputExpression.pop_back();
                } else if (c == '\r')  // Enter key
                {
                    if (!inputExpression.empty()) {
                        expression = inputExpression;
                        generateExpressionTruthTable();
                        showInputField = false;
                        inputExpression.clear();
                    }
                } else if (std::isalpha(c) || c == '.' || c == '+' || c == '~' || c == '^' || c == '(' || c == ')') {
                    inputExpression += c;
                }
                setupUITexts();
            }
        }
    }
}

void Simulator::addInput(sf::Vector2f position) { gates.emplace_back(GateType::INPUT, position); }

void Simulator::clearCircuit() {
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
    showInputField = false;
    inputExpression.clear();
    expression.clear();
    variables.clear();
    varValues.clear();
    minterms.clear();
    inputCounter = 0;
    outputCounter = 0;
    std::cout << "Circuit cleared!" << std::endl;
}

void Simulator::selectGateAt(sf::Vector2f worldPos) {
    // Clear previous selections
    for (auto &gate : gates) {
        gate.setSelected(false);
    }
    selectedGates.clear();

    // Select gate at position
    for (size_t i = 0; i < gates.size(); ++i) {
        if (gates[i].getBounds().contains(worldPos)) {
            gates[i].setSelected(true);
            selectedGates.push_back(i);
            break;
        }
    }
}

void Simulator::deleteSelectedGates() {
    if (selectedGates.empty()) return;

    // Sort indices in descending order to avoid index shifting issues
    std::sort(selectedGates.begin(), selectedGates.end(), std::greater<size_t>());

    for (size_t gateIndex : selectedGates) {
        if (gateIndex < gates.size()) {
            removeGate(gateIndex);
        }
    }

    selectedGates.clear();
    std::cout << "Deleted selected gates" << std::endl;
}

void Simulator::cancelSelection() {
    // Cancel wire placement
    selectedGate = std::numeric_limits<size_t>::max();
    selectedPin = -1;
    selectingSource = true;

    // Clear gate selections
    for (auto &gate : gates) {
        gate.setSelected(false);
    }
    selectedGates.clear();
}

void Simulator::removeGate(size_t gateIndex) {
    if (gateIndex >= gates.size()) return;

    // Remove all wires connected to this gate
    removeWiresConnectedToGate(gateIndex);

    // Remove the gate
    gates.erase(gates.begin() + gateIndex);

    // Update wire indices (shift down by 1 for gates with index > gateIndex)
    for (auto &wire : wires) {
        if (wire.getSrcGate() > gateIndex) {
            wire = Wire(wire.getSrcGate() - 1, wire.getSrcPin(), wire.getDstGate(), wire.getDstPin());
        }
        if (wire.getDstGate() > gateIndex) {
            wire = Wire(wire.getSrcGate(), wire.getSrcPin(), wire.getDstGate() - 1, wire.getDstPin());
        }
    }
}

void Simulator::removeWiresConnectedToGate(size_t gateIndex) {
    wires.erase(std::remove_if(wires.begin(), wires.end(),
                               [gateIndex](const Wire &wire) { return wire.getSrcGate() == gateIndex || wire.getDstGate() == gateIndex; }),
                wires.end());
}

void Simulator::update() {
    // Update wire positions
    for (auto &wire : wires) {
        if (wire.getSrcGate() < gates.size() && wire.getDstGate() < gates.size()) {
            sf::Vector2f start = gates[wire.getSrcGate()].getOutputPinPosition();
            sf::Vector2f end = gates[wire.getDstGate()].getInputPinPosition(wire.getDstPin());
            wire.setPositions(start, end);
        }
    }
    evaluateCircuit();
}

void Simulator::evaluateCircuit() {
    // Reset all gate states except inputs
    for (auto &gate : gates) {
        if (gate.getType() != GateType::INPUT) {
            gate.setState(false);
        }
    }

    // Topological evaluation
    std::vector<bool> evaluated(gates.size(), false);
    bool progress = true;

    while (progress) {
        progress = false;
        for (size_t i = 0; i < gates.size(); ++i) {
            if (evaluated[i] || gates[i].getType() == GateType::INPUT) {
                if (gates[i].getType() == GateType::INPUT) evaluated[i] = true;
                continue;
            }

            // Check if all inputs are available
            std::vector<bool> inputs;
            int inputCount = (gates[i].getType() == GateType::NOT) ? 1 : 2;
            bool allInputsReady = true;

            for (int j = 0; j < inputCount; ++j) {
                bool inputValue = false;
                bool inputFound = false;

                for (const auto &wire : wires) {
                    if (wire.getDstGate() == i && wire.getDstPin() == j && wire.getSrcGate() < gates.size()) {
                        if (evaluated[wire.getSrcGate()]) {
                            inputValue = gates[wire.getSrcGate()].getState();
                            inputFound = true;
                            break;
                        } else {
                            allInputsReady = false;
                            break;
                        }
                    }
                }

                if (!allInputsReady) break;
                inputs.push_back(inputValue);
            }

            if (allInputsReady && inputs.size() == inputCount) {
                gates[i].setState(gates[i].evaluate(inputs));
                evaluated[i] = true;
                progress = true;
            }
        }
    }
}

void Simulator::draw(sf::RenderWindow &window) const {
    for (const auto &gate : gates) {
        gate.draw(window);
    }
    for (const auto &wire : wires) {
        wire.draw(window);
    }

    // Draw selection indicators
    if (selectedGate < gates.size()) {
        sf::CircleShape indicator(12.f);
        indicator.setFillColor(sf::Color::Transparent);
        indicator.setOutlineThickness(2.f);
        indicator.setOutlineColor(sf::Color::Cyan);
        sf::Vector2f pos = gates[selectedGate].getOutputPinPosition();
        indicator.setPosition(pos - sf::Vector2f{12.f, 12.f});
        window.draw(indicator);
    }
}

void Simulator::drawUI(sf::RenderWindow &window) const {
    sf::View originalView = window.getView();
    sf::View uiView({600.f, 400.f}, {1200.f, 800.f});
    window.setView(uiView);

    float rightSide = 820.f;  // Right side of the UI

    // Draw simplified expression
    if (showExpression && currentFont && expressionTitleText && expressionText) {
        sf::RectangleShape expressionBg({380.f, 120.f});
        expressionBg.setPosition({rightSide, 50.f});
        expressionBg.setFillColor(sf::Color(0, 0, 0, 180));
        expressionBg.setOutlineThickness(2.f);
        expressionBg.setOutlineColor(sf::Color::White);
        window.draw(expressionBg);

        // Draw title
        expressionTitleText->setPosition({rightSide + 10.f, 60.f});
        window.draw(*expressionTitleText);

        // Draw expression
        expressionText->setPosition({rightSide + 10.f, 80.f});
        window.draw(*expressionText);

        // Draw close button
        sf::RectangleShape closeBtn({60.f, 20.f});
        closeBtn.setPosition({rightSide + 310.f, 55.f});
        closeBtn.setFillColor(sf::Color::Red);
        closeBtn.setOutlineThickness(1.f);
        closeBtn.setOutlineColor(sf::Color::White);
        window.draw(closeBtn);

        sf::Text closeText(*currentFont);
        closeText.setString("CLOSE");
        closeText.setCharacterSize(12);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition({rightSide + 320.f, 60.f});
        window.draw(closeText);

        // Instructions
        sf::Text instructText(*currentFont);
        instructText.setString("Press E again or click CLOSE to hide");
        instructText.setCharacterSize(10);
        instructText.setFillColor(sf::Color(128, 128, 128));
        instructText.setPosition({rightSide + 10.f, 140.f});
        window.draw(instructText);
    }

    // Draw truth table
    if (showTruthTable && !truthTable.empty() && currentFont && truthTableTitleText) {
        float tableHeight = std::min(400.f, (float)truthTable.size() * 20.f + 80.f);
        sf::RectangleShape tableBg({380.f, tableHeight});
        tableBg.setPosition({rightSide, 200.f});
        tableBg.setFillColor(sf::Color(0, 0, 0, 180));
        tableBg.setOutlineThickness(2.f);
        tableBg.setOutlineColor(sf::Color::White);
        window.draw(tableBg);

        // Draw title
        truthTableTitleText->setPosition({rightSide + 10.f, 210.f});
        window.draw(*truthTableTitleText);

        // Draw close button
        sf::RectangleShape closeBtn({60.f, 20.f});
        closeBtn.setPosition({rightSide + 310.f, 205.f});
        closeBtn.setFillColor(sf::Color::Red);
        closeBtn.setOutlineThickness(1.f);
        closeBtn.setOutlineColor(sf::Color::White);
        window.draw(closeBtn);

        sf::Text closeText(*currentFont);
        closeText.setString("CLOSE");
        closeText.setCharacterSize(12);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition({rightSide + 320.f, 210.f});
        window.draw(closeText);

        // Draw table headers and data
        float yPos = 230.f;
        for (size_t i = 0; i < std::min((size_t)15, truthTableTexts.size()); ++i) {
            truthTableTexts[i].setPosition({rightSide + 10.f, yPos});
            window.draw(truthTableTexts[i]);
            yPos += 18.f;
        }

        if (truthTable.size() > 15) {
            sf::Text moreText(*currentFont);
            moreText.setString("... (showing first 15 rows)");
            moreText.setCharacterSize(10);
            moreText.setFillColor(sf::Color(128, 128, 128));
            moreText.setPosition({rightSide + 10.f, yPos});
            window.draw(moreText);
        }

        // Instructions
        sf::Text instructText(*currentFont);
        instructText.setString("Press T again or click CLOSE to hide");
        instructText.setCharacterSize(10);
        instructText.setFillColor(sf::Color(128, 128, 128));
        instructText.setPosition({rightSide + 10.f, 200.f + tableHeight - 20.f});
        window.draw(instructText);
    }

    // Draw input field
    if (showInputField && currentFont && inputFieldText) {
        if (!inputFieldBg) {
            inputFieldBg = std::make_unique<sf::RectangleShape>(sf::Vector2f{380.f, 120.f});
            inputFieldBg->setPosition({rightSide, 350.f});
            inputFieldBg->setFillColor(sf::Color(0, 0, 0, 180));
            inputFieldBg->setOutlineThickness(2.f);
            inputFieldBg->setOutlineColor(sf::Color::White);
        }
        window.draw(*inputFieldBg);

        inputFieldText->setPosition({rightSide + 10.f, 360.f});
        window.draw(*inputFieldText);

        sf::Text instructText(*currentFont);
        instructText.setString("Enter expression (use . for AND, + for OR, ~ for NOT, ^ for XOR)\nPress Enter to submit, or click CLOSE to hide");
        instructText.setCharacterSize(10);
        instructText.setFillColor(sf::Color(128, 128, 128));
        instructText.setPosition({rightSide + 10.f, 400.f});
        window.draw(instructText);

        // Draw close button
        sf::RectangleShape closeBtn({60.f, 20.f});
        closeBtn.setPosition({rightSide + 310.f, 355.f});
        closeBtn.setFillColor(sf::Color::Red);
        closeBtn.setOutlineThickness(1.f);
        closeBtn.setOutlineColor(sf::Color::White);
        window.draw(closeBtn);

        sf::Text closeText(*currentFont);
        closeText.setString("CLOSE");
        closeText.setCharacterSize(12);
        closeText.setFillColor(sf::Color::White);
        closeText.setPosition({rightSide + 320.f, 360.f});
        window.draw(closeText);
    }

    window.setView(originalView);
}

std::vector<size_t> Simulator::getInputGates() const {
    std::vector<size_t> inputs;
    for (size_t i = 0; i < gates.size(); ++i) {
        if (gates[i].getType() == GateType::INPUT) {
            inputs.push_back(i);
        }
    }
    return inputs;
}

std::vector<size_t> Simulator::getOutputGates() const {
    std::vector<size_t> outputs;
    for (size_t i = 0; i < gates.size(); ++i) {
        if (gates[i].getType() == GateType::OUTPUT) {
            outputs.push_back(i);
        }
    }
    return outputs;
}

void Simulator::generateLogicalExpression() {
    if (showExpression) {
        showExpression = false;
        return;
    }

    auto outputs = getOutputGates();
    if (outputs.empty()) {
        currentExpression = "No OUTPUT gates found";
        showExpression = true;
        std::cout << currentExpression << std::endl;
        return;
    }

    std::map<size_t, std::string> expressions;
    auto inputs = getInputGates();

    for (size_t i = 0; i < inputs.size(); ++i) {
        char varName = 'A' + i;
        expressions[inputs[i]] = std::string(1, varName);
    }

    currentExpression = generateExpressionForGate(outputs[0], expressions);
    showExpression = true;
    setupUITexts();

    std::cout << "Generated expression: " << currentExpression << std::endl;
}

std::string Simulator::generateExpressionForGate(size_t gateIndex, std::map<size_t, std::string> &expressions) const {
    if (expressions.find(gateIndex) != expressions.end()) {
        return expressions[gateIndex];
    }

    if (gateIndex >= gates.size()) {
        return "?";
    }

    const Gate &gate = gates[gateIndex];
    std::string result;

    switch (gate.getType()) {
        case GateType::INPUT:
            result = "INPUT";
            break;

        case GateType::NOT: {
            for (const auto &wire : wires) {
                if (wire.getDstGate() == gateIndex && wire.getDstPin() == 0) {
                    std::string input = generateExpressionForGate(wire.getSrcGate(), expressions);
                    result = "¬(" + input + ")";
                    break;
                }
            }
            if (result.empty()) result = "¬(?)";
            break;
        }

        case GateType::AND: {
            std::vector<std::string> inputs;
            for (int pin = 0; pin < 2; ++pin) {
                for (const auto &wire : wires) {
                    if (wire.getDstGate() == gateIndex && wire.getDstPin() == pin) {
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

        case GateType::OR: {
            std::vector<std::string> inputs;
            for (int pin = 0; pin < 2; ++pin) {
                for (const auto &wire : wires) {
                    if (wire.getDstGate() == gateIndex && wire.getDstPin() == pin) {
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

        case GateType::OUTPUT: {
            for (const auto &wire : wires) {
                if (wire.getDstGate() == gateIndex && wire.getDstPin() == 0) {
                    result = generateExpressionForGate(wire.getSrcGate(), expressions);
                    break;
                }
            }
            if (result.empty()) result = "?";
            break;
        }
    }

    expressions[gateIndex] = result;
    return result;
}

void Simulator::generateTruthTable() {
    if (showTruthTable) {
        showTruthTable = false;
        return;
    }

    auto inputs = getInputGates();
    auto outputs = getOutputGates();

    if (inputs.empty() || outputs.empty()) {
        truthTable = {"No inputs or outputs found"};
        showTruthTable = true;
        std::cout << "Cannot generate truth table: No inputs or outputs found" << std::endl;
        return;
    }

    truthTable.clear();

    std::string header;
    for (size_t i = 0; i < inputs.size(); ++i) {
        header += "In" + std::to_string(i) + "  ";
    }
    header += " | ";
    for (size_t i = 0; i < outputs.size(); ++i) {
        header += "Out" + std::to_string(i) + " ";
    }
    truthTable.push_back(header);

    std::string separator(header.length(), '-');
    truthTable.push_back(separator);

    int numInputs = inputs.size();
    int combinations = 1 << numInputs;

    for (int combo = 0; combo < combinations; ++combo) {
        for (int i = 0; i < numInputs; ++i) {
            bool value = (combo >> i) & 1;
            gates[inputs[i]].setState(value);
        }

        const_cast<Simulator *>(this)->evaluateCircuit();

        std::string row;
        for (int i = 0; i < numInputs; ++i) {
            row += (gates[inputs[i]].getState() ? " 1" : " 0");
            row += "   ";
        }
        row += " | ";
        for (size_t i = 0; i < outputs.size(); ++i) {
            row += (gates[outputs[i]].getState() ? "  1" : "  0");
            row += "   ";
        }
        truthTable.push_back(row);
    }

    showTruthTable = true;
    setupUITexts();

    std::cout << "Generated truth table with " << combinations << " rows" << std::endl;
}

void Simulator::generateExpressionTruthTable() {
    if (expression.empty()) {
        truthTable = {"No expression entered"};
        showTruthTable = true;
        std::cout << "No expression entered!" << std::endl;
        return;
    }

    variables.clear();
    for (char c : expression) {
        if (std::isalpha(c)) {
            variables.push_back(c);
        }
    }
    removeDuplicateVariables();

    std::string postfix = infixToPostfix(expression);
    int varCount = variables.size();
    int rows = 1 << varCount;
    minterms.clear();
    truthTable.clear();

    std::string header;
    for (char var : variables) {
        header += std::string(1, var) + "  ";
    }
    header += "| " + expression;
    truthTable.push_back(header);

    std::string separator(header.length(), '-');
    truthTable.push_back(separator);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < varCount; j++) {
            varValues[variables[j]] = (i >> (varCount - 1 - j)) & 1;
        }

        bool result = evaluatePostfix(postfix);
        minterms.push_back(result);

        std::string row;
        for (int j = 0; j < varCount; j++) {
            row += (varValues[variables[j]] ? " 1" : " 0") + std::string("  ");
        }
        row += "|  " + std::string(result ? "1" : "0");
        truthTable.push_back(row);
    }

    showTruthTable = true;
    currentExpression = simplifyExpression();
    showExpression = true;
    setupUITexts();

    std::cout << "Generated truth table for expression: " << expression << std::endl;
    std::cout << "Simplified expression: " << currentExpression << std::endl;
}

void Simulator::setFont(const sf::Font &font) {
    currentFont = &font;
    setupUITexts();
}

void Simulator::setupUITexts() const {
    if (currentFont == nullptr) {
        std::cout << "Error: currentFont is null in setupUITexts" << std::endl;
        return;
    }

    expressionTitleText = std::make_unique<sf::Text>(*currentFont);
    expressionTitleText->setString("Simplified Expression:");
    expressionTitleText->setCharacterSize(16);
    expressionTitleText->setFillColor(sf::Color::White);
    std::cout << "setupUITexts: Initialized expressionTitleText" << std::endl;

    expressionText = std::make_unique<sf::Text>(*currentFont);
    expressionText->setString(currentExpression.empty() ? "No expression" : currentExpression);
    expressionText->setCharacterSize(14);
    expressionText->setFillColor(sf::Color::Yellow);
    std::cout << "setupUITexts: expressionText set to: " << (currentExpression.empty() ? "No expression" : currentExpression) << std::endl;

    truthTableTitleText = std::make_unique<sf::Text>(*currentFont);
    truthTableTitleText->setString("Truth Table:");
    truthTableTitleText->setCharacterSize(16);
    truthTableTitleText->setFillColor(sf::Color::White);
    std::cout << "setupUITexts: Initialized truthTableTitleText" << std::endl;

    truthTableTexts.clear();
    for (const auto &row : truthTable) {
        sf::Text rowText(*currentFont);
        rowText.setString(row);
        rowText.setCharacterSize(12);
        rowText.setFillColor(sf::Color::White);
        truthTableTexts.push_back(rowText);
    }
    std::cout << "setupUITexts: Initialized " << truthTableTexts.size() << " truth table rows" << std::endl;

    inputFieldText = std::make_unique<sf::Text>(*currentFont);
    inputFieldText->setString(inputExpression.empty() ? "Enter expression..." : inputExpression);
    inputFieldText->setCharacterSize(14);
    inputFieldText->setFillColor(sf::Color::White);
    std::cout << "setupUITexts: Initialized inputFieldText" << std::endl;
}

void Simulator::toggleInputField() {
    showInputField = !showInputField;
    if (!showInputField) {
        inputExpression.clear();
    }
    setupUITexts();
}

void Simulator::removeDuplicateVariables() {
    std::sort(variables.begin(), variables.end());
    variables.erase(std::unique(variables.begin(), variables.end()), variables.end());
}

bool Simulator::isOperator(char c) { return c == '.' || c == '+' || c == '~' || c == '^'; }

int Simulator::precedence(char op) {
    if (op == '+') {
        return 1;
    }
    if (op == '.') {
        return 2;
    }
    if (op == '~' || op == '^') {
        return 3;
    }
    return 0;
}

bool Simulator::applyOperation(bool a, bool b, char op) {
    switch (op) {
        case '.':
            return a && b;
        case '+':
            return a || b;
        case '^':
            return a != b;
        default:
            return false;
    }
}

std::string Simulator::infixToPostfix(const std::string &infix) {
    std::stack<char> s;
    std::string postfix;

    for (char c : infix) {
        if (std::isalpha(c)) {
            postfix += c;
        } else if (c == '(') {
            s.push(c);
        } else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop();
        } else if (isOperator(c)) {
            while (!s.empty() && precedence(s.top()) >= precedence(c)) {
                postfix += s.top();
                s.pop();
            }
            s.push(c);
        }
    }

    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

bool Simulator::evaluatePostfix(const std::string &postfix) {
    std::stack<bool> s;

    for (char c : postfix) {
        if (std::isalpha(c)) {
            s.push(varValues[c]);
        } else if (c == '~') {
            bool a = s.top();
            s.pop();
            s.push(!a);
        } else {
            bool b = s.top();
            s.pop();
            bool a = s.top();
            s.pop();
            s.push(applyOperation(a, b, c));
        }
    }

    return s.top();
}

std::string Simulator::getBinaryString(int num, int length) {
    std::string binary;
    for (int i = length - 1; i >= 0; i--) {
        binary += ((num >> i) & 1) ? '1' : '0';
    }
    return binary;
}

bool Simulator::differsByOneBit(const std::string &a, const std::string &b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) diff++;
        if (diff > 1) return false;
    }
    return diff == 1;
}

std::string Simulator::combineTerms(const std::string &a, const std::string &b) {
    std::string combined;
    for (size_t i = 0; i < a.size(); i++) {
        combined += (a[i] == b[i]) ? a[i] : '-';
    }
    return combined;
}

std::string Simulator::termToExpression(const std::string &term) {
    std::string expr;
    for (size_t i = 0; i < term.size(); i++) {
        if (term[i] == '0')
            expr += "~" + std::string(1, variables[i]) + " . ";
        else if (term[i] == '1')
            expr += std::string(1, variables[i]) + " . ";
    }
    if (!expr.empty()) expr = expr.substr(0, expr.size() - 3);
    return expr.empty() ? "1" : expr;
}

std::string Simulator::simplifyExpression() {
    if (variables.empty() || minterms.empty()) return "0";

    bool all_true = true, all_false = true;
    for (bool m : minterms) {
        if (!m) all_true = false;
        if (m) all_false = false;
    }
    if (all_true) return "1";
    if (all_false) return "0";

    std::set<std::string> binaryMinterms;
    for (size_t i = 0; i < minterms.size(); i++) {
        if (minterms[i]) {
            binaryMinterms.insert(getBinaryString(i, variables.size()));
        }
    }

    std::set<std::string> primeImplicants = binaryMinterms;
    bool changed;
    do {
        changed = false;
        std::set<std::string> used;
        std::set<std::string> newImplicants;

        auto it1 = primeImplicants.begin();
        while (it1 != primeImplicants.end()) {
            bool combined = false;
            auto it2 = primeImplicants.begin();
            while (it2 != primeImplicants.end()) {
                if (it1 == it2) {
                    ++it2;
                    continue;
                }
                if (differsByOneBit(*it1, *it2)) {
                    newImplicants.insert(combineTerms(*it1, *it2));
                    used.insert(*it1);
                    used.insert(*it2);
                    combined = true;
                    changed = true;
                }
                ++it2;
            }
            if (!combined && used.find(*it1) == used.end()) {
                newImplicants.insert(*it1);
            }
            ++it1;
        }

        primeImplicants = newImplicants;
    } while (changed);

    std::string result;
    for (const std::string &term : primeImplicants) {
        std::string expr = termToExpression(term);
        if (!expr.empty()) result += "(" + expr + ") + ";
    }

    if (!result.empty()) result = result.substr(0, result.size() - 3);

    if (primeImplicants.size() == 2) {
        auto it = primeImplicants.begin();
        std::string term1 = *it;
        std::string term2 = *(++it);

        bool isXOR = true;
        for (size_t i = 0; i < term1.size(); i++) {
            if ((term1[i] == '0' && term2[i] != '1') || (term1[i] == '1' && term2[i] != '0') || (term2[i] == '0' && term1[i] != '1') ||
                (term2[i] == '1' && term1[i] != '0')) {
                isXOR = false;
                break;
            }
        }
        if (isXOR) {
            return std::string(1, variables[0]) + " ^ " + std::string(1, variables[1]);
        }
    }

    return result.empty() ? "0" : result;
}

void Simulator::readExpression() {
    // Implemented via UI input field, not console
}

void Simulator::generateCircuitFromExpression(const std::string &expr) {
    // Placeholder for future implementation
}