#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <limits>
#include "Gate.h"
#include "Wire.h"

class Simulator
{
public:
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGate);
    void update();
    void draw(sf::RenderWindow &window) const;
    void drawUI(sf::RenderWindow &window) const;
    void addInput(sf::Vector2f position);

    // New functionality
    void generateTruthTable();
    void generateLogicalExpression();
    void clearCircuit();
    void deleteSelectedGates();
    void cancelSelection();
    void selectGateAt(sf::Vector2f worldPos);
    void setFont(const sf::Font &font);

private:
    std::vector<Gate> gates;
    std::vector<Wire> wires;
    size_t selectedGate = std::numeric_limits<size_t>::max();
    int selectedPin = -1;
    bool selectingSource = true;
    std::vector<size_t> selectedGates; // Multiple selection support

    // UI elements for truth table and expression
    std::string currentExpression;
    std::vector<std::string> truthTable;
    bool showTruthTable = false;
    bool showExpression = false;

    // Font for UI text rendering
    const sf::Font *currentFont = nullptr;
    mutable std::vector<sf::Text> truthTableTexts;
    mutable std::unique_ptr<sf::Text> expressionText;
    mutable std::unique_ptr<sf::Text> expressionTitleText;
    mutable std::unique_ptr<sf::Text> truthTableTitleText;

    // Input/Output counters for unique naming
    int inputCounter = 0;
    int outputCounter = 0;

    void evaluateCircuit();
    std::vector<size_t> getInputGates() const;
    std::vector<size_t> getOutputGates() const;
    std::string generateExpressionForGate(size_t gateIndex, std::map<size_t, std::string> &expressions) const;
    std::string getGateSymbol(GateType type) const;
    void removeGate(size_t gateIndex);
    void removeWiresConnectedToGate(size_t gateIndex);
    void setupUITexts() const;
};