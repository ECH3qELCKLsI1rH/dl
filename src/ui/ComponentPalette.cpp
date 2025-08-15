// ui/ComponentPalette.cpp

#include "ComponentPalette.h"

#include <iostream>

#include "Configuration.h"

// =================== UI CONSTANTS =================== //
namespace {

// Variables
const float LEFT_MARGIN = 50.f;  // left margin
const float TOP_MARGIN = 40.f;   // vertical space
const float SPACING = 25.f;      // vertical space

// Spacing
const float BOX_WIDTH = 180.f;                         // box width
const float BOX_HEIGHT = 60.f;                         // box height
const float BOX_Y_SPACING = BOX_HEIGHT + SPACING;      // distance between boxes
const float BOX_Y_START = BOX_Y_SPACING + TOP_MARGIN;  // first button Y start

// Font Sizes
const unsigned int FONT_TITLE = 20;
const unsigned int FONT_LABEL = 18;
const unsigned int FONT_INSTRUCTION = 18;

// Colors
const sf::Color COLOR_SELECTED = sf::Color::Yellow;
const sf::Color COLOR_HOVERED = sf::Color(220, 220, 220);
const sf::Color COLOR_DEFAULT = sf::Color::White;
const sf::Color COLOR_OUTLINE = sf::Color::Black;

const sf::Color COLOR_BG = sf::Color(200, 200, 200, 200);
const sf::Color COLOR_TITLE_BG = sf::Color::Black;
const sf::Color COLOR_TITLE_TEXT = sf::Color::White;
const sf::Color COLOR_LABEL_TEXT = sf::Color::Black;
const sf::Color COLOR_INSTR_TEXT = sf::Color::Black;

const float OUTLINE_THICKNESS = 2.f;

}  // namespace

ComponentPalette::ComponentPalette() {
    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();
    uiView.setSize(paletteSize);
    uiView.setCenter(paletteSize / 2.f);
    uiView.setViewport(sf::FloatRect({0, 0}, {WindowConfig::PALETTE_SCALE, 1}));

    type = {GateType::INPUT, GateType::AND, GateType::OR, GateType::NOT, GateType::NAND, GateType::NOR, GateType::EXOR, GateType::OUTPUT};
    setupButtons();
}

void ComponentPalette::setupButtons() {
    buttons.clear();

    for (size_t i = 0; i < type.size(); ++i) {
        sf::RectangleShape button({BOX_WIDTH, BOX_HEIGHT});
        button.setPosition({LEFT_MARGIN, BOX_Y_START + i * BOX_Y_SPACING});

        if (i == selectedIndex)
            button.setFillColor(COLOR_SELECTED);
        else if (i == hoveredIndex)
            button.setFillColor(COLOR_HOVERED);
        else
            button.setFillColor(COLOR_DEFAULT);

        button.setOutlineThickness(OUTLINE_THICKNESS);
        button.setOutlineColor(COLOR_OUTLINE);

        buttons.push_back(button);
    }

    if (currentFont != nullptr) setupTexts();
}

void ComponentPalette::setFont(const sf::Font &font) {
    currentFont = &font;
    setupTexts();
}

void ComponentPalette::setupTexts() {
    if (!currentFont) return;

    buttonLabels.clear();
    instructionTexts.clear();

    // Button labels
    for (size_t i = 0; i < type.size(); ++i) {
        sf::Text label(*currentFont);
        label.setString(getGateTypeName(type[i]));
        label.setCharacterSize(FONT_LABEL);
        label.setFillColor(COLOR_LABEL_TEXT);

        sf::Vector2f pos = buttons[i].getPosition();
        sf::Vector2f size = buttons[i].getSize();
        sf::FloatRect bounds = label.getLocalBounds();

        label.setPosition({pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x, pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y});

        buttonLabels.push_back(label);
    }

    // Title text
    titleText = sf::Text(*currentFont);
    titleText->setString("Components");
    titleText->setCharacterSize(FONT_TITLE);
    titleText->setFillColor(COLOR_TITLE_TEXT);

    sf::FloatRect tBounds = titleText->getLocalBounds();
    titleText->setPosition({LEFT_MARGIN + (BOX_WIDTH - tBounds.size.x) / 2.f - tBounds.position.x,
                            TOP_MARGIN + (BOX_HEIGHT - tBounds.size.y) / 2.f - tBounds.position.y});

    // Instructions
    float instrStartY = BOX_Y_START + type.size() * BOX_Y_SPACING + SPACING;
    std::vector<std::string> instructions = {"CONTROLS:",   "T      Truth Table", "E      Expression", "I       Input Expression",
                                             "C     Clear", "Del   Delete",       "Q     Quit"};

    for (size_t i = 0; i < instructions.size(); ++i) {
        sf::Text instr(*currentFont);
        instr.setString(instructions[i]);
        instr.setCharacterSize(FONT_INSTRUCTION);
        instr.setFillColor(COLOR_INSTR_TEXT);
        instr.setPosition({LEFT_MARGIN, instrStartY + i * SPACING});
        instructionTexts.push_back(instr);
    }
}

void ComponentPalette::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, uiView);
        int newHovered = -1;

        if (mousePos.x >= LEFT_MARGIN && mousePos.x <= LEFT_MARGIN + BOX_WIDTH) {
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].getGlobalBounds().contains(mousePos)) {
                    newHovered = static_cast<int>(i);
                    break;
                }
            }
        }

        if (newHovered != hoveredIndex) {
            hoveredIndex = newHovered;
            setupButtons();
        }
    }

    if (auto clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (clicked->button == sf::Mouse::Button::Left) {
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            sf::Vector2f pos = window.mapPixelToCoords(mp, uiView);
            if (pos.x >= LEFT_MARGIN && pos.x <= LEFT_MARGIN + BOX_WIDTH) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(pos)) {
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
    sf::RectangleShape bg(WindowConfig::getPaletteSize());
    bg.setFillColor(COLOR_BG);
    bg.setPosition({0.f, 0.f});
    window.draw(bg);

    // Title background
    sf::RectangleShape titleBg({BOX_WIDTH, BOX_HEIGHT});
    titleBg.setPosition({LEFT_MARGIN, BOX_Y_START - BOX_HEIGHT - SPACING});
    titleBg.setFillColor(COLOR_TITLE_BG);
    titleBg.setOutlineThickness(OUTLINE_THICKNESS);
    titleBg.setOutlineColor(COLOR_OUTLINE);
    window.draw(titleBg);

    if (currentFont && titleText.has_value()) window.draw(titleText.value());

    // Buttons
    for (size_t i = 0; i < buttons.size(); ++i) {
        window.draw(buttons[i]);
        if (currentFont && i < buttonLabels.size()) window.draw(buttonLabels[i]);
    }

    // Instructions
    if (currentFont) {
        for (auto &instr : instructionTexts) window.draw(instr);
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
