#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

enum class GateType { AND, OR, NOT, NAND, NOR, XOR, INPUT, OUTPUT };

class Gate {
   private:
    GateType type;
    sf::Vector2f position;
    sf::RectangleShape shape;
    bool state = false;
    bool selected = false;
    std::vector<Gate*> inputGates;

    const sf::Font* font = nullptr;
    std::optional<sf::Text> text;

    void drawGateLabel(sf::RenderWindow& window) const;

   public:
    Gate(GateType type, sf::Vector2f position);

    // Input connections
    void addInput(Gate* input) { inputGates.push_back(input); }
    void removeInput(Gate* input) { inputGates.erase(std::remove(inputGates.begin(), inputGates.end(), input), inputGates.end()); }
    const std::vector<Gate*>& getInputs() const { return inputGates; }

    int getInputCount() const;

    // Evaluation
    bool evaluate() const;
    bool evaluate(const std::vector<bool>& inputs) const;

    // Drawing
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getInputPinPosition(int index) const;
    sf::Vector2f getOutputPinPosition() const;
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

    // State
    void setState(bool val);
    bool getState() const { return state; }

    // Type
    GateType getType() const { return type; }
    std::string getGateTypeString() const;

    // Selection
    void setSelected(bool val) { selected = val; }

    // Font
    void setFont(sf::Font& f) { font = &f; }
};
