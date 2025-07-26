#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../engine/Gate.h"

class ComponentPalette
{
public:
    ComponentPalette();

    void handleEvent(const sf::Event &event);
    void update();
    void draw(sf::RenderWindow &window);

    GateType getSelectedGateType() const;

private:
    std::vector<sf::RectangleShape> buttons;
    std::vector<GateType> gateTypes;
    int selectedIndex = -1;
    sf::View uiView; // Fixed UI view for palette
};