#include "Canvas.h"
#include <cmath>

Canvas::Canvas()
{
    view.setCenter({400.f, 300.f});
    view.setSize({800.f, 600.f});
}

void Canvas::handleEvent(const sf::Event &event)
{
    // Zoom with mouse wheel
    if (const auto *wheel = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        float factor = (wheel->delta > 0) ? 0.9f : 1.1f;
        view.setSize(view.getSize() * factor);
    }

    // Middle mouse button to start dragging
    if (const auto *pressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (pressed->button == sf::Mouse::Button::Middle)
        {
            dragging = true;
            lastMousePos = sf::Vector2f{(float)pressed->position.x, (float)pressed->position.y};
        }
    }

    // Release drag
    if (const auto *released = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (released->button == sf::Mouse::Button::Middle)
        {
            dragging = false;
        }
    }

    // Dragging (pan)
    if (const auto *moved = event.getIf<sf::Event::MouseMoved>())
    {
        if (dragging)
        {
            sf::Vector2f newPos{(float)moved->position.x, (float)moved->position.y};
            sf::Vector2f delta = lastMousePos - newPos;
            view.move(delta);
            lastMousePos = newPos;
        }
    }
}

void Canvas::update()
{
    simulator.update();
}

void Canvas::draw(sf::RenderWindow &window)
{
    window.setView(view);
    drawGrid(window);
    simulator.draw(window);
}

void Canvas::drawGrid(sf::RenderWindow &window)
{
    const float gridSize = 50.f;
    sf::Vector2f size = view.getSize();
    sf::Vector2f center = view.getCenter();

    float left = center.x - size.x / 2.f;
    float right = center.x + size.x / 2.f;
    float top = center.y - size.y / 2.f;
    float bottom = center.y + size.y / 2.f;

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (float x = std::floor(left / gridSize) * gridSize; x <= right; x += gridSize)
    {
        lines.append({{x, top}, sf::Color(60, 60, 60)});
        lines.append({{x, bottom}, sf::Color(60, 60, 60)});
    }
    for (float y = std::floor(top / gridSize) * gridSize; y <= bottom; y += gridSize)
    {
        lines.append({{left, y}, sf::Color(60, 60, 60)});
        lines.append({{right, y}, sf::Color(60, 60, 60)});
    }

    window.draw(lines);
}