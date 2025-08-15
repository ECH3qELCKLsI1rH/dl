#include "ComponentPalette.h"

#include <iostream>

#include "Configuration.h"

ComponentPalette::ComponentPalette() {
    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();

    // Create view for the palette area
    uiView.setSize(paletteSize);
    uiView.setCenter(paletteSize / 2.f);
    uiView.setViewport(sf::FloatRect({0, 0}, {WindowConfig::PALETTE_SCALE, 1}));

    type = {GateType::INPUT, GateType::AND, GateType::OR, GateType::NOT, GateType::OUTPUT};
    setupButtons();
}

void ComponentPalette::setupButtons() {
    buttons.clear();

    for (int i = 0; i < type.size(); ++i) {
        sf::RectangleShape button({80.f, 40.f});      // for input, and, or, not, output of size (80x40)px
        button.setPosition({20.f, 50.f + i * 50.f});  // set position 20px right and 50px below the top for every button
        if (i == selectedIndex) {
            button.setFillColor(sf::Color::Yellow);  // if button is selected, yellow
        } else if (i == hoveredIndex) {
            button.setFillColor(sf::Color(220, 220, 220));  // if button is hovered, gray
        } else {
            button.setFillColor(sf::Color::White);  // default color
        }
        button.setOutlineThickness(2.f);           // button border thickness
        button.setOutlineColor(sf::Color::Black);  // button border color
        buttons.push_back(button);
    }

    if (currentFont != nullptr) {
        setupTexts();
    }
}

void ComponentPalette::setFont(const sf::Font &font) {
    currentFont = &font;
    setupTexts();
}

void ComponentPalette::setupTexts() {
    if (currentFont == nullptr) return;

    buttonLabels.clear();
    instructionTexts.clear();

    for (size_t i = 0; i < type.size(); ++i) {
        sf::Text label(*currentFont);
        label.setString(getGateTypeName(type[i]));
        label.setCharacterSize(14);
        label.setFillColor(sf::Color::Black);

        sf::Vector2f buttonPos = buttons[i].getPosition();
        sf::Vector2f buttonSize = buttons[i].getSize();
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setPosition({buttonPos.x + (buttonSize.x - textBounds.size.x) / 2.f, buttonPos.y + (buttonSize.y - textBounds.size.y) / 2.f});

        buttonLabels.push_back(label);
    }

    titleText = sf::Text(*currentFont);
    titleText->setString("Components");
    titleText->setCharacterSize(16);
    titleText->setFillColor(sf::Color::White);
    titleText->setPosition({15.f, 15.f});

    std::vector<std::string> instructions = {"Controls:", "T-Truth Table", "E-Expression", "I-Input Expr", "F-File Menu",
                                             "C-Clear",   "Del-Delete",    "Esc-Cancel",   "Q-Quit"};

    for (size_t i = 0; i < instructions.size(); ++i) {
        sf::Text instrText(*currentFont);
        instrText.setString(instructions[i]);
        instrText.setCharacterSize(12);
        instrText.setFillColor(sf::Color::Black);
        instrText.setPosition({15.f, 350.f + i * 20.f});
        instructionTexts.push_back(instrText);
    }
}

void ComponentPalette::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, uiView);

        int newHoveredIndex = -1;
        if (mousePos.x >= 20.f && mousePos.x <= 100.f) {
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].getGlobalBounds().contains(mousePos)) {
                    newHoveredIndex = static_cast<int>(i);
                    break;
                }
            }
        }

        if (newHoveredIndex != hoveredIndex) {
            hoveredIndex = newHoveredIndex;
            setupButtons();
        }
    }

    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (clicked->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, uiView);

            if (mousePos.x >= 20.f && mousePos.x <= 100.f) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) {
                        selectedIndex = static_cast<int>(i);
                        setupButtons();
                        break;
                    }
                }
            }
        }
    }
}

void ComponentPalette::update() { setupButtons(); }

void ComponentPalette::draw(sf::RenderWindow &window) {
    window.setView(uiView);

    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();
    sf::RectangleShape background(paletteSize);
    background.setPosition({0.f, 0.f});
    background.setFillColor(sf::Color(200, 200, 200, 200));
    window.draw(background);

    sf::RectangleShape titleBg({100.f, 30.f});
    titleBg.setPosition({10.f, 10.f});
    titleBg.setFillColor(sf::Color::Black);
    titleBg.setOutlineThickness(1.f);
    titleBg.setOutlineColor(sf::Color::White);
    window.draw(titleBg);

    if (currentFont != nullptr && titleText.has_value()) {
        window.draw(titleText.value());
    }

    for (size_t i = 0; i < buttons.size(); ++i) {
        window.draw(buttons[i]);
        if (currentFont != nullptr && i < buttonLabels.size()) {
            window.draw(buttonLabels[i]);
        }
    }

    if (currentFont != nullptr) {
        for (const auto &instrText : instructionTexts) {
            window.draw(instrText);
        }
    }
}

std::string ComponentPalette::getGateTypeName(GateType type) const {
    switch (type) {
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

GateType ComponentPalette::getSelectedGateType() const { return type[selectedIndex]; }
