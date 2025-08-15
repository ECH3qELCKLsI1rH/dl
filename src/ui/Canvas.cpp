#include "Canvas.h"

#include <cmath>

#include "Configuration.h"

Canvas::Canvas(Simulator &simulator) : simulator(simulator) {
    sf::Vector2f canvasSize = WindowConfig::getCanvasSize();
    sf::Vector2f paletteSize = WindowConfig::getPaletteSize();

    // Position the view after the palette
    view.setSize(canvasSize);
    view.setCenter({paletteSize.x + canvasSize.x / 2.f, canvasSize.y / 2.f});
}

void Canvas::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (const auto *wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        float factor = (wheel->delta > 0) ? 0.9f : 1.1f;  // zoom in/out
        view.setSize(view.getSize() * factor);
    }

    if (const auto *pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Middle) {
            dragging = true;
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            initialPos = sf::Vector2f{(float)mousePixel.x, (float)mousePixel.y};
        }
    }

    if (const auto *released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Middle) {
            dragging = false;
        }
    }

    if (const auto *moved = event.getIf<sf::Event::MouseMoved>()) {
        if (dragging) {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f finalPos{(float)mousePixel.x, (float)mousePixel.y};
            sf::Vector2f difference = initialPos - finalPos;
            float zoomFactor = view.getSize().x / window.getSize().x;
            difference *= zoomFactor;
            view.move(difference);
            initialPos = finalPos;
        }
    }
}

void Canvas::update() { simulator.update(); }

void Canvas::drawGrid(sf::RenderWindow &window) {
    const float gridSize = 50.f;
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    float left = center.x - size.x / 2.f;
    float right = center.x + size.x / 2.f;
    float top = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    // Draw vertical grid lines
    for (float x = std::floor(left / gridSize) * gridSize; x <= right; x += gridSize) {
        lines.append({{x, top}, sf::Color(60, 60, 60)});
        lines.append({{x, bottom}, sf::Color(60, 60, 60)});
    }

    // Draw horizontal grid lines
    for (float y = std::floor(top / gridSize) * gridSize; y <= bottom; y += gridSize) {
        lines.append({{left, y}, sf::Color(60, 60, 60)});
        lines.append({{right, y}, sf::Color(60, 60, 60)});
    }

    window.draw(lines);
}

void Canvas::draw(sf::RenderWindow &window) {
    window.setView(view);
    drawGrid(window);
    simulator.draw(window);
}
