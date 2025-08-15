// engine/Gate.cpp

#include "Gate.h"

Gate::Gate(GateType type, sf::Vector2f position) : type(type), position(position) {
    shape.setSize({50.f, 50.f});              // gate ko shape = (50x50)px ko square
    shape.setPosition(position);              // sets the position of the shape (square) to a 2d co-ordinate
    shape.setOutlineThickness(2.f);           // shape's border ko thickness = 2px
    shape.setOutlineColor(sf::Color::Black);  // shape's border ko color = Black

    switch (type) {  // to fill the square with different colors based on component type (ip/op/logic gates)
        case GateType::INPUT:
        case GateType::OUTPUT:
            shape.setFillColor(sf::Color(128, 128, 128));  // fill the shape with color = Gray
            break;
        case GateType::AND:
        case GateType::OR:
        case GateType::NOT:
            shape.setFillColor(sf::Color(200, 200, 200));  // Light gray
            break;
    }
}

void Gate::draw(sf::RenderWindow &window) const {
    sf::RectangleShape gateShape = shape;  // data member 'shape' ko copy so the original doesnt get affected
    if (selected) {                        // "kunai gate selected chha bhane do the following"
        gateShape.setOutlineThickness(4.f);
        gateShape.setOutlineColor(sf::Color::Yellow);
    }
    window.draw(gateShape);  // window ma draw garne (with updates if selected else normal wala)

    drawGateLabel(window);                  // gate (square) center ma text lekhna
    sf::CircleShape pin(6.f);               // pin ko radius = 6px
    pin.setOutlineThickness(1.f);           // pin ko border thickness = 1px
    pin.setOutlineColor(sf::Color::Black);  // pin ko border color = Black

    if (type != GateType::OUTPUT) {
        pin.setFillColor((state == true) ? (sf::Color::Red) : (sf::Color::White));  // if state is HIGH, pin is red else white
        pin.setPosition(getOutputPinPosition() - sf::Vector2f{6.f, 6.f});
        // OutputPinPosition ma center hune gari set gareko (accounting radius of the pin)
        window.draw(pin);
    }

    if (type != GateType::INPUT) {
        int inputCount = (type == GateType::NOT || type == GateType::OUTPUT) ? 1 : 2;
        pin.setFillColor(sf::Color::White);
        for (int i = 0; i < inputCount; ++i) {
            pin.setPosition(getInputPinPosition(i) - sf::Vector2f{6.f, 6.f});  // fixing pin position ma offset d/t its radius
            window.draw(pin);
        }
    }
}

bool Gate::evaluate(const std::vector<bool> &inputs) const {
    switch (type) {
        case GateType::AND:
            // only return input[0] && input[1] if exactly 2 inputs are present
            return inputs.size() == 2 && (inputs[0] && inputs[1]);
        case GateType::OR:
            return inputs.size() == 2 && (inputs[0] || inputs[1]);
        case GateType::NOT:
            return inputs.size() == 1 && !inputs[0];
        case GateType::INPUT:
            return state;
        case GateType::OUTPUT:  // if one or more inputs are given, return the state of the input that was connected first
            return (inputs.size() >= 1) ? inputs[0] : false;
        default:
            return false;
    }
}

// returns a rectangle that fully contains the shape in window coordinates
sf::FloatRect Gate::getBounds() const { return shape.getGlobalBounds(); }

sf::Vector2f Gate::getInputPinPosition(int pinIndex) const {  // returns input pins ko coordinates
    if (type == GateType::NOT || type == GateType::OUTPUT) {
        return position + sf::Vector2f{0.f, 25.f};  // single input pin chha bhane left border ko center ma
    }
    return position + sf::Vector2f{0.f, (pinIndex == 0) ? 15.f : 35.f};  // double input pins chha bhane right most border ko lagi 0.f ani
    // mathi ko pin 15px below from top border, tala ko pin 15 px mathi from bottom border
}

sf::Vector2f Gate::getOutputPinPosition() const {  // returns output pin ko coordinates
    return position + sf::Vector2f{50.f, 25.f};    // Output pin always at right border ko center ma
}

void Gate::setState(bool state) {  // change the state of a gate to the one that's passed in the argument
    this->state = state;
    if (type == GateType::INPUT || type == GateType::OUTPUT) {  // Red for high, gray for low
        shape.setFillColor((state == true) ? (sf::Color::Red) : (sf::Color(128, 128, 128)));
    }
}

bool Gate::getState() const { return state; }  // returns either true->HIGH or false->LOW

GateType Gate::getType() const { return type; }  // returns AND / OR / NOT / INPUT / OUTPUT

std::string Gate::getGateTypeString() const {  // returns string based on gate ko type
    switch (type) {
        case GateType::INPUT:
            return "IN";
        case GateType::OUTPUT:
            return "OUT";
        case GateType::AND:
            return "AND";
        case GateType::OR:
            return "OR";
        case GateType::NOT:
            return "NOT";
        default:
            return "?";
    }
}

void Gate::drawGateLabel(sf::RenderWindow &window) const {
    std::string label = getGateTypeString();
    sf::Vector2f center = position + sf::Vector2f{25.f, 25.f};  // coordinate of gate (square) ko center
    float charWidth = 6.f;                                      // Width per character (5 pixels + 1 for spacing)
    float charHeight = 7.f;                                     // Height of character
    float totalWidth = label.length() * charWidth - 1.f;        // -1 to remove last character ko pachhadi ko space
    float startX = center.x - totalWidth / 2.f;                 // center bata left tira space banaune
    float startY = center.y - charHeight / 2.f;                 // center bata mathi tira space banaune

    for (int i = 0; i < label.length(); ++i) {
        drawCharacters(window, label[i], startX + i * charWidth, startY);
        // each pixel letter art always starts from top so "startY" but
        // X-axis ma position right tira sardai jaanchha so "startX + i * charWidth"
    }
}

void Gate::drawCharacters(sf::RenderWindow &window, char c, float x, float y) const {  // x, y are positions
    // (4+1)x7 pixel patterns for each character (+1 for spacing)
    std::vector<std::vector<int>> pattern;  // 2x2 vector 0/1 pixels for the character

    switch (std::toupper(c)) {  // 1 -> draw pixel, 0 -> dont draw pixel
        case 'A':
            pattern = {{0, 1, 1, 0}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}};
            break;
        case 'N':
            pattern = {{1, 0, 0, 1}, {1, 1, 0, 1}, {1, 1, 0, 1}, {1, 0, 1, 1}, {1, 0, 1, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}};
            break;
        case 'D':
            pattern = {{1, 1, 1, 0}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}};
            break;
        case 'O':
            pattern = {{0, 1, 1, 0}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 1, 0}};
            break;
        case 'R':
            pattern = {{1, 1, 1, 0}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 1, 0}, {1, 0, 0, 1}};
            break;
        case 'I':
            pattern = {{1, 1, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {1, 1, 1}};
            break;
        case 'U':
            pattern = {{1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 1, 0}};
            break;
        case 'T':
            pattern = {{1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}};
            break;
        default:
            pattern = {{0}};  // "dont draw pixels for unknown characters"
            break;
    }

    for (int row = 0; row < pattern.size(); ++row) {  // loop that iterates over each row and column
        for (int col = 0; col < pattern[row].size(); ++col) {
            if (pattern[row][col] == 1) {               // if 1 draw pixel else dont do anything
                sf::RectangleShape pixel({1.f, 1.f});   // "pixel" of size (1x1)px
                pixel.setPosition({x + col, y + row});  // {x, y} (top left corner) ko respect ma pixel ko position set gareko
                pixel.setFillColor(sf::Color::Black);
                window.draw(pixel);
            }
        }
    }
}

void Gate::setSelected(bool isSelected) { selected = isSelected; }  // change the "selected" state of a gate