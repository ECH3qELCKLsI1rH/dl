#include "ComponentPalette.h"
#include <iostream>

ComponentPalette::ComponentPalette()
{
    // Initialize view for UI rendering (fixed UI view)
    uiView = sf::View({600.f, 400.f}, {1200.f, 800.f});
    gateTypes = {GateType::INPUT, GateType::AND, GateType::OR, GateType::NOT, GateType::OUTPUT};

    setupButtons();
}

void ComponentPalette::setupButtons()
{
    buttons.clear();

    for (size_t i = 0; i < gateTypes.size(); ++i)
    {
        sf::RectangleShape button({80.f, 40.f});
        button.setPosition({20.f, 50.f + i * 50.f});
        button.setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        button.setOutlineThickness(2.f);
        button.setOutlineColor(sf::Color::Black);
        buttons.push_back(button);
    }
}

std::string ComponentPalette::getGateTypeName(GateType type) const
{
    switch (type)
    {
    case GateType::INPUT:
        return "INPUT";
    case GateType::AND:
        return "AND";
    case GateType::OR:
        return "OR";
    case GateType::NOT:
        return "NOT";
    case GateType::OUTPUT:
        return "OUTPUT";
    default:
        return "UNKNOWN";
    }
}

void ComponentPalette::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};

            // Check if click is in palette area
            if (mousePos.x <= 120.f)
            {
                for (size_t i = 0; i < buttons.size(); ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos))
                    {
                        selectedIndex = i;
                        setupButtons(); // Refresh button colors
                        break;
                    }
                }
            }
        }
    }
}

void ComponentPalette::update()
{
    // No update logic needed
}

void ComponentPalette::draw(sf::RenderWindow &window)
{
    window.setView(uiView);

    // Draw palette background
    sf::RectangleShape background({120.f, 800.f});
    background.setPosition({0.f, 0.f});
    background.setFillColor(sf::Color(200, 200, 200, 200));
    window.draw(background);

    // Draw title
    sf::RectangleShape titleBg({100.f, 30.f});
    titleBg.setPosition({10.f, 10.f});
    titleBg.setFillColor(sf::Color::Black);
    titleBg.setOutlineThickness(1.f);
    titleBg.setOutlineColor(sf::Color::White);
    window.draw(titleBg);

    // Draw buttons and labels
    for (size_t i = 0; i < buttons.size(); ++i)
    {
        window.draw(buttons[i]);

        // Draw button label at center
        sf::Vector2f buttonPos = buttons[i].getPosition();
        sf::Vector2f buttonSize = buttons[i].getSize();

        // Create simple text-like rectangles since we can't use sf::Text
        std::string gateName = getGateTypeName(gateTypes[i]);
        drawSimpleText(window, gateName,
                       sf::Vector2f(buttonPos.x + buttonSize.x / 2 - gateName.length() * 3,
                                    buttonPos.y + buttonSize.y / 2 - 5),
                       sf::Color::Black);
    }

    // Draw instructions
    drawSimpleText(window, "Controls:", sf::Vector2f(15.f, 350.f), sf::Color::Black);
    drawSimpleText(window, "T-Truth Table", sf::Vector2f(15.f, 370.f), sf::Color::Black);
    drawSimpleText(window, "E-Expression", sf::Vector2f(15.f, 390.f), sf::Color::Black);
    drawSimpleText(window, "C-Clear", sf::Vector2f(15.f, 410.f), sf::Color::Black);
    drawSimpleText(window, "Del-Delete", sf::Vector2f(15.f, 430.f), sf::Color::Black);
    drawSimpleText(window, "Esc-Cancel", sf::Vector2f(15.f, 450.f), sf::Color::Black);
    drawSimpleText(window, "F-File Menu", sf::Vector2f(15.f, 470.f), sf::Color::Black);
}

GateType ComponentPalette::getSelectedGateType() const
{
    return gateTypes[selectedIndex];
}

void ComponentPalette::drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color)
{
    // Draw simple pixel-based text using rectangles
    for (size_t i = 0; i < text.length(); ++i)
    {
        if (text[i] != ' ' && text[i] != '-')
        {
            sf::RectangleShape charRect({6.f, 8.f});
            charRect.setPosition({position.x + i * 7.f, position.y});
            charRect.setFillColor(color);
            window.draw(charRect);
        }
    }
}
