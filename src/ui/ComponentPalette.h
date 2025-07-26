#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "../engine/Gate.h"

class ComponentPalette
{
public:
    ComponentPalette();

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);
    void setFont(const sf::Font &font);

    GateType getSelectedGateType() const;

private:
    sf::View uiView;
    std::vector<GateType> gateTypes;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonLabels;
    std::vector<sf::Text> instructionTexts;
    std::optional<sf::Text> titleText;
    int selectedIndex = 0;
    int hoveredIndex = -1;
    const sf::Font *currentFont = nullptr;

    void setupButtons();
    void setupTexts();
    std::string getGateTypeName(GateType type) const;
};