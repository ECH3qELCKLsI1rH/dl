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

        // Set color based on state: selected (yellow), hovered (light gray), normal (white)
        if (i == selectedIndex)
        {
            button.setFillColor(sf::Color::Yellow);
        }
        else if (i == hoveredIndex)
        {
            button.setFillColor(sf::Color(220, 220, 220)); // Light gray for hover
        }
        else
        {
            button.setFillColor(sf::Color::White);
        }

        button.setOutlineThickness(2.f);
        button.setOutlineColor(sf::Color::Black);
        buttons.push_back(button);
    }

    // Setup texts if font is available
    if (currentFont != nullptr)
    {
        setupTexts();
    }
}

void ComponentPalette::setFont(const sf::Font &font)
{
    currentFont = &font;
    setupTexts();
}

void ComponentPalette::setupTexts()
{
    if (currentFont == nullptr)
        return;

    // Clear existing texts
    buttonLabels.clear();
    instructionTexts.clear();

    // Setup button labels
    for (size_t i = 0; i < gateTypes.size(); ++i)
    {
        sf::Text label(*currentFont);
        label.setString(getGateTypeName(gateTypes[i]));
        label.setCharacterSize(14);
        label.setFillColor(sf::Color::Black);

        // Center the label in the button
        sf::Vector2f buttonPos = buttons[i].getPosition();
        sf::Vector2f buttonSize = buttons[i].getSize();
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setPosition({buttonPos.x + (buttonSize.x - textBounds.size.x) / 2,
                           buttonPos.y + (buttonSize.y - textBounds.size.y) / 2});

        buttonLabels.push_back(label);
    }

    // Setup title text
    titleText = sf::Text(*currentFont);
    titleText->setString("Components");
    titleText->setCharacterSize(16);
    titleText->setFillColor(sf::Color::White);
    titleText->setPosition({15.f, 15.f});

    // Setup instruction texts
    std::vector<std::string> instructions = {
        "Controls:",
        "T-Truth Table",
        "E-Expression",
        "C-Clear",
        "Del-Delete",
        "Esc-Cancel",
        "F-File Menu"};

    for (size_t i = 0; i < instructions.size(); ++i)
    {
        sf::Text instrText(*currentFont);
        instrText.setString(instructions[i]);
        instrText.setCharacterSize(12);
        instrText.setFillColor(sf::Color::Black);
        instrText.setPosition({15.f, 350.f + i * 20.f});
        instructionTexts.push_back(instrText);
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
    // Always update hover state based on mouse position
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos{(float)mousePixel.x, (float)mousePixel.y};

    // Update hover state
    int newHoveredIndex = -1;
    if (mousePos.x <= 120.f) // Only hover in palette area
    {
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            if (buttons[i].getGlobalBounds().contains(mousePos))
            {
                newHoveredIndex = static_cast<int>(i);
                break;
            }
        }
    }

    // Update hover state and refresh buttons if it changed
    if (newHoveredIndex != hoveredIndex)
    {
        hoveredIndex = newHoveredIndex;
        setupButtons(); // Refresh button colors
    }

    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
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

    // Draw title background
    sf::RectangleShape titleBg({100.f, 30.f});
    titleBg.setPosition({10.f, 10.f});
    titleBg.setFillColor(sf::Color::Black);
    titleBg.setOutlineThickness(1.f);
    titleBg.setOutlineColor(sf::Color::White);
    window.draw(titleBg);

    // Draw title text
    if (currentFont != nullptr && titleText.has_value())
    {
        window.draw(titleText.value());
    }

    // Draw buttons and labels
    for (size_t i = 0; i < buttons.size(); ++i)
    {
        window.draw(buttons[i]);

        // Draw button label using proper text
        if (currentFont != nullptr && i < buttonLabels.size())
        {
            window.draw(buttonLabels[i]);
        }
    }

    // Draw instructions using proper text
    if (currentFont != nullptr)
    {
        for (const auto &instrText : instructionTexts)
        {
            window.draw(instrText);
        }
    }
}

GateType ComponentPalette::getSelectedGateType() const
{
    return gateTypes[selectedIndex];
}
