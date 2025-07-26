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

private:
    std::vector<Gate> gates;
    std::vector<Wire> wires;
};