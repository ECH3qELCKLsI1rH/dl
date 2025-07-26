#include "ComponentPalette.h"

ComponentPalette::ComponentPalette()
{
    uiView = sf::View({400.f, 300.f}, {800.f, 600.f});
    gateTypes = {GateType::AND, GateType::OR, GateType::NOT};
    for (size_t i = 0; i < gateTypes.size(); ++i)
    {
        sf::RectangleShape button({50.f, 50.f});
        button.setPosition({10.f, 10.f + i * 60.f});
        button.setFillColor(sf::Color::White);
        buttons.push_back(button);
    }
}

void ComponentPalette::handleEvent(const sf::Event &event)
{
    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};
            for (size_t i = 0; i < buttons.size(); ++i)
            {
                if (buttons[i].getGlobalBounds().contains(mousePos))
                {
                    selectedIndex = i;
                    buttons[i].setFillColor(sf::Color::Yellow);
                    for (size_t j = 0; j < buttons.size(); ++j)
                    {
                        if (j != i)
                            buttons[j].setFillColor(sf::Color::White);
                    }
                }
            }
        }
    }
}

void ComponentPalette::update()
{
    // No update logic needed for now
}

void ComponentPalette::draw(sf::RenderWindow &window)
{
    window.setView(uiView);
    for (const auto &button : buttons)
    {
        window.draw(button);
    }
}

GateType ComponentPalette::getSelectedGateType() const
{
    return (selectedIndex >= 0) ? gateTypes[selectedIndex] : GateType::AND;
}