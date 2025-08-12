#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Gate.h"
#include "Wire.h"

class Simulator {
   public:
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGate);
    void update();
    void draw(sf::RenderWindow &window) const;
    void drawUI(sf::RenderWindow &window) const;
    void addInput(sf::Vector2f position);
    void readExpression();
    void generateTruthTable();
    void generateExpressionTruthTable();
    std::string simplifyExpression();  // Changed from void to std::string
    void generateLogicalExpression();
    void clearCircuit();
    void deleteSelectedGates();
    void cancelSelection();
    void selectGateAt(sf::Vector2f worldPos);
    void setFont(const sf::Font &font);
    void toggleInputField();
    bool isInputFieldActive() const { return showInputField; }

   private:
    std::string expression;
    std::vector<char> variables;
    std::map<char, bool> varValues;
    std::vector<bool> minterms;

    void removeDuplicateVariables();
    bool isOperator(char c);
    int precedence(char op);
    bool applyOperation(bool a, bool b, char op);
    std::string infixToPostfix(const std::string &infix);
    bool evaluatePostfix(const std::string &postfix);
    std::string getBinaryString(int num, int length);
    bool differsByOneBit(const std::string &a, const std::string &b);
    std::string combineTerms(const std::string &a, const std::string &b);
    std::string termToExpression(const std::string &term);
    void generateCircuitFromExpression(const std::string &expr);  // Changed from std::string to void

    std::vector<Gate> gates;
    std::vector<Wire> wires;
    size_t selectedGate = std::numeric_limits<size_t>::max();
    int selectedPin = -1;
    bool selectingSource = true;
    std::vector<size_t> selectedGates;

    std::string currentExpression;
    std::vector<std::string> truthTable;
    bool showTruthTable = false;
    bool showExpression = false;
    bool showInputField = false;
    std::string inputExpression;
    mutable std::unique_ptr<sf::Text> inputFieldText;
    mutable std::unique_ptr<sf::RectangleShape> inputFieldBg;

    const sf::Font *currentFont = nullptr;
    mutable std::vector<sf::Text> truthTableTexts;
    mutable std::unique_ptr<sf::Text> expressionText;
    mutable std::unique_ptr<sf::Text> expressionTitleText;
    mutable std::unique_ptr<sf::Text> truthTableTitleText;

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