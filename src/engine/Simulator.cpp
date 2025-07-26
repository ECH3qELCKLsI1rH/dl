#include "Simulator.h"

void Simulator::handleEvent(const sf::Event &event, const sf::RenderWindow &window, const sf::View &view, GateType selectedGate)
{
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            // Convert screen coordinates to world coordinates
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(mousePos), view);
            gates.emplace_back(selectedGate, worldPos);
        }
    }
}

void Simulator::update()
{
    // Placeholder for simulation logic (e.g., evaluate gate outputs)
}

void Simulator::draw(sf::RenderWindow &window) const
{
    for (const auto &gate : gates)
    {
        gate.draw(window);
    }
    for (const auto &wire : wires)
    {
        wire.draw(window);
    }
}