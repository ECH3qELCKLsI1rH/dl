#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Gate.h"
#include "Wire.h"

class Simulator
{
public:
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGate);
    void update();
    void draw(sf::RenderWindow &window) const;
    void addInput(sf::Vector2f position);

private:
    std::vector<Gate> gates;
    std::vector<Wire> wires;
    size_t selectedGate = std::numeric_limits<size_t>::max(); // Index of selected gate
    int selectedPin = -1;
    bool selectingSource = true;

    void evaluateCircuit();
};