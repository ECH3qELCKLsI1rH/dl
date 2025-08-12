#include "Wire.h"

// constructor
Wire::Wire(size_t srcGate, int srcPin, size_t dstGate, int dstPin) : srcGate(srcGate), srcPin(srcPin), dstGate(dstGate), dstPin(dstPin) {
    line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    // like saying "create a pair of points and set it to draw them as a Line/LineStrip"
    line[0].color = sf::Color::Yellow;  // line ko starting laai yellow color banaaune
    line[1].color = sf::Color::Yellow;  // line ko ending laai yellow color banaaune (different bhayo bhane gradient aauchha)
}

void Wire::draw(sf::RenderWindow &window) const {  // draws the wire on a given sf::RenderWindow
    // drawing a line three times thatre shifted vertically by 'offset' pixels
    for (int offset = -1; offset <= 1; ++offset) {
        sf::VertexArray thickLine = line;  // data member 'line' ko copy so the original doesnt get affected
        thickLine[0].position.y += offset;
        thickLine[1].position.y += offset;
        window.draw(thickLine);  // window ma draw garne
    }
}

void Wire::setPositions(sf::Vector2f start, sf::Vector2f end) {  // to update the start and end positions of the wire
    line[0].position = start;                                    // line ko vertices ma naya 2d coordinated haru assign gareko
    line[1].position = end;                                      // Vecfor2 le 2D coordinates  store garchha in floating-point pixels
}

size_t Wire::getSrcGate() const { return srcGate; }
int Wire::getSrcPin() const { return srcPin; }
size_t Wire::getDstGate() const { return dstGate; }
int Wire::getDstPin() const { return dstPin; }