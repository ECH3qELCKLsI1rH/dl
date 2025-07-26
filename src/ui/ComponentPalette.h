#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../engine/Gate.h"

class ComponentPalette
{
public:
    ComponentPalette();

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);

    GateType getSelectedGateType() const;

private:
    sf::View uiView;
    std::vector<GateType> gateTypes;
    std::vector<sf::RectangleShape> buttons;
    int selectedIndex = 0;

    void setupButtons();
    std::string getGateTypeName(GateType type) const;
    void drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color);
};