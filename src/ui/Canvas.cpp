#include "Canvas.h"

#include <cmath>

Canvas::Canvas(Simulator &simulator) : simulator(simulator) {
    view.setCenter({600.f, 400.f});
    view.setSize({1200.f, 800.f});
}

void Canvas::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    // Zoom with mouse wheel
    if (const auto *wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        float factor = (wheel->delta > 0) ? 0.9f : 1.1f;
        view.setSize(view.getSize() * factor);
    }

    // Middle mouse button to start dragging
    if (const auto *pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (pressed->button == sf::Mouse::Button::Middle) {
            dragging = true;
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            lastMousePos = sf::Vector2f{(float)mousePixel.x, (float)mousePixel.y};
        }
    }

    // Release drag
    if (const auto *released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Middle) {
            dragging = false;
        }
    }

    // Dragging (pan)
    if (const auto *moved = event.getIf<sf::Event::MouseMoved>()) {
        if (dragging) {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f newPos{(float)mousePixel.x, (float)mousePixel.y};
            sf::Vector2f delta = lastMousePos - newPos;

            // Scale delta by the zoom level
            float zoomFactor = view.getSize().x / window.getSize().x;
            delta *= zoomFactor;

            view.move(delta);
            lastMousePos = newPos;
        }
    }
}

void Canvas::update() { simulator.update(); }

void Canvas::draw(sf::RenderWindow &window) {
    window.setView(view);
    drawGrid(window);
    simulator.draw(window);
}

void Canvas::drawGrid(sf::RenderWindow &window) {
    const float gridSize = 50.f;
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    float left = center.x - size.x / 2.f;
    float right = center.x + size.x / 2.f;
    float top = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (float x = std::floor(left / gridSize) * gridSize; x <= right; x += gridSize) {
        lines.append({{x, top}, sf::Color(60, 60, 60)});
        lines.append({{x, bottom}, sf::Color(60, 60, 60)});
    }
    for (float y = std::floor(top / gridSize) * gridSize; y <= bottom; y += gridSize) {
        lines.append({{left, y}, sf::Color(60, 60, 60)});
        lines.append({{right, y}, sf::Color(60, 60, 60)});
    }

    window.draw(lines);
}