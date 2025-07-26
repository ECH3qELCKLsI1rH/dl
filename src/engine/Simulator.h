#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
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
    void drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color) const;
};