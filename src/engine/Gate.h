// engine/Gate.h

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class GateType { AND, OR, NOT, NAND, NOR, EXOR, INPUT, OUTPUT };

class Gate {
   private:
    GateType type;
    sf::Vector2f position;
    sf::RectangleShape shape;
    bool state = false;  // false->LOW and true->HIGH
    bool selected = false;

    void drawGateLabel(sf::RenderWindow &window) const;
    void drawCharacters(sf::RenderWindow &window, char c, float x, float y) const;

   public:
    Gate(GateType type, sf::Vector2f position);
    void draw(sf::RenderWindow &window) const;
    bool evaluate(const std::vector<bool> &inputs) const;
    sf::Vector2f getInputPinPosition(int index) const;
    sf::Vector2f getOutputPinPosition() const;
    void setState(bool state);

    // Getter functions
    sf::FloatRect getBounds() const;
    bool getState() const;
    GateType getType() const;
    std::string getGateTypeString() const;

    // Selection methods
    void setSelected(bool isSelected);
};