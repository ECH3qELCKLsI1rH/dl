#include "ComponentPalette.h"

#include <iostream>

#include "Configuration.h"

// =================== UI CONSTANTS =================== //
namespace {
// Palette dimensions
const float BUTTON_WIDTH = 120.f;
const float BUTTON_HEIGHT = 60.f;
const float BUTTON_X_OFFSET = 50.f;   // from left
const float BUTTON_Y_START = 50.f;    // first button vertical start
const float BUTTON_Y_SPACING = 60.f;  // distance between buttons

// Title bar
const float TITLE_BG_WIDTH = 200.f;
const float TITLE_BG_HEIGHT = 60.f;
const float TITLE_X = 50.f;
const float TITLE_Y = 20.f;
const unsigned int TITLE_FONT_SIZE = 20;

// Text
const unsigned int LABEL_FONT_SIZE = 20;
const unsigned int INSTRUCTION_FONT_SIZE = 16;

// Instruction positioning
const float INSTRUCTION_X = 20.f;
const float INSTRUCTION_Y_START = 500.f;
const float INSTRUCTION_Y_SPACING = 20.f;

// Colors
const sf::Color COLOR_SELECTED = sf::Color::Yellow;
const sf::Color COLOR_HOVERED(220, 220, 220);
const sf::Color COLOR_DEFAULT = sf::Color::White;
const sf::Color COLOR_BUTTON_OUTLINE = sf::Color::Black;
const float BUTTON_OUTLINE_THICKNESS = 2.f;

const sf::Color COLOR_TITLE_BG = sf::Color::Black;
const sf::Color COLOR_TITLE_OUTLINE = sf::Color::White;
const sf::Color COLOR_TITLE_TEXT = sf::Color::White;
const sf::Color COLOR_LABEL_TEXT = sf::Color::Black;
const sf::Color COLOR_INSTRUCTION_TEXT = sf::Color::Black;
const sf::Color COLOR_BACKGROUND(200, 200, 200, 200);
}  // namespace

ComponentPalette::ComponentPalette() {
    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();

    // Create view for the palette area
    uiView.setSize(paletteSize);
    uiView.setCenter(paletteSize / 2.f);
    uiView.setViewport(sf::FloatRect({0, 0}, {WindowConfig::PALETTE_SCALE, 1}));

    type = {GateType::INPUT, GateType::AND, GateType::OR, GateType::NOT, GateType::NAND, GateType::NOR, GateType::EXOR, GateType::OUTPUT};
    setupButtons();
}

void ComponentPalette::setupButtons() {
    buttons.clear();

    for (int i = 0; i < static_cast<int>(type.size()); ++i) {
        sf::RectangleShape button({BUTTON_WIDTH, BUTTON_HEIGHT});
        button.setPosition({BUTTON_X_OFFSET, BUTTON_Y_START + i * BUTTON_Y_SPACING});
        if (i == selectedIndex) {
            button.setFillColor(COLOR_SELECTED);  // if button is selected, yellow
        } else if (i == hoveredIndex) {
            button.setFillColor(COLOR_HOVERED);  // if button is hovered, gray
        } else {
            button.setFillColor(COLOR_DEFAULT);  // default color
        }
        button.setOutlineThickness(BUTTON_OUTLINE_THICKNESS);  // button border thickness
        button.setOutlineColor(COLOR_BUTTON_OUTLINE);          // button border color
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
        label.setCharacterSize(LABEL_FONT_SIZE);
        label.setFillColor(COLOR_LABEL_TEXT);

        sf::Vector2f buttonPos = buttons[i].getPosition();
        sf::Vector2f buttonSize = buttons[i].getSize();
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setPosition({buttonPos.x + (buttonSize.x - textBounds.size.x) / 2.f, buttonPos.y + (buttonSize.y - textBounds.size.y) / 2.f});

        buttonLabels.push_back(label);
    }

    titleText = sf::Text(*currentFont);
    titleText->setString("Components");
    titleText->setCharacterSize(TITLE_FONT_SIZE);
    titleText->setFillColor(COLOR_TITLE_TEXT);
    titleText->setPosition({BUTTON_X_OFFSET, TITLE_Y});

    std::vector<std::string> instructions = {"Controls:",   "T-Truth Table", "E-Expression", "I-Input Expr",
                                             "F-File Menu", "C-Clear",       "Del-Delete",   "Q-Quit"};

    for (size_t i = 0; i < instructions.size(); ++i) {
        sf::Text instrText(*currentFont);
        instrText.setString(instructions[i]);
        instrText.setCharacterSize(INSTRUCTION_FONT_SIZE);
        instrText.setFillColor(COLOR_INSTRUCTION_TEXT);
        instrText.setPosition({INSTRUCTION_X, INSTRUCTION_Y_START + i * INSTRUCTION_Y_SPACING});
        instructionTexts.push_back(instrText);
    }
}

void ComponentPalette::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, uiView);

        int newHoveredIndex = -1;
        if (mousePos.x >= BUTTON_X_OFFSET && mousePos.x <= BUTTON_X_OFFSET + BUTTON_WIDTH) {
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

            if (mousePos.x >= BUTTON_X_OFFSET && mousePos.x <= BUTTON_X_OFFSET + BUTTON_WIDTH) {
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

    // Background
    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();
    sf::RectangleShape background(paletteSize);
    background.setPosition({0.f, 0.f});
    background.setFillColor(COLOR_BACKGROUND);
    window.draw(background);

    // Title Background
    sf::RectangleShape titleBg({TITLE_BG_WIDTH, TITLE_BG_HEIGHT});
    titleBg.setPosition({TITLE_X, TITLE_Y});
    titleBg.setFillColor(COLOR_TITLE_BG);
    titleBg.setOutlineThickness(1.f);
    titleBg.setOutlineColor(COLOR_TITLE_OUTLINE);
    window.draw(titleBg);

    // Title
    if (currentFont != nullptr && titleText.has_value()) {
        window.draw(titleText.value());
    }

    // Buttons
    for (size_t i = 0; i < buttons.size(); ++i) {
        window.draw(buttons[i]);
        if (currentFont != nullptr && i < buttonLabels.size()) {
            window.draw(buttonLabels[i]);
        }
    }

    // Instructions
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
        case GateType::NAND:
            return "NAND";
        case GateType::NOR:
            return "NOR";
        case GateType::EXOR:
            return "EXOR";
        case GateType::OUTPUT:
            return "OUTPUT";
        default:
            return "UNKNOWN";
    }
}

GateType ComponentPalette::getSelectedGateType() const { return type[selectedIndex]; }
