#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

#include "../engine/Gate.h"

class ComponentPalette {
   public:
    ComponentPalette();
    void setFont(const sf::Font &font);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);
    GateType getSelectedGateType() const;

   private:
    void setupButtons();
    void setupTexts();
    std::string getGateTypeName(GateType type) const;

    std::vector<sf::RectangleShape> buttons;
    std::vector<GateType> gateTypes;
    int selectedIndex = 0;
    int hoveredIndex = -1;
    sf::View uiView;
    const sf::Font *currentFont = nullptr;
    std::optional<sf::Text> titleText;
    std::vector<sf::Text> buttonLabels;
    std::vector<sf::Text> instructionTexts;
};