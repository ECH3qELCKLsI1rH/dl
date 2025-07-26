#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class GateType
{
    AND,
    OR,
    NOT,
    INPUT,
    OUTPUT
};

class Gate
{
public:
    Gate(GateType type, sf::Vector2f position);

    void draw(sf::RenderWindow &window) const;
    bool evaluate(const std::vector<bool> &inputs) const;
    sf::FloatRect getBounds() const;

    sf::Vector2f getInputPinPosition(int index) const;
    sf::Vector2f getOutputPinPosition() const;
    void setState(bool state);
    bool getState() const;
    GateType getType() const;
    std::string getGateTypeString() const;

    // Selection methods
    void setSelected(bool isSelected);
    bool isSelected() const;

private:
    GateType type;
    sf::Vector2f position;
    sf::RectangleShape shape;
    bool state = false;    // For INPUT & OUTPUT gates
    bool selected = false; // For selection highlighting

    void drawGateLabel(sf::RenderWindow &window) const;
    void drawCharacter(sf::RenderWindow &window, char c, float x, float y) const;
    void drawCharacterPixels(sf::RenderWindow &window, char c, float x, float y) const;
};