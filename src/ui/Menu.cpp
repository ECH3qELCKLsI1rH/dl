#include "Menu.h"

Menu::Menu()
{
    setupMenu();
}

void Menu::setupMenu()
{
    // Menu background
    menuBackground.setSize({300.f, 200.f});
    menuBackground.setPosition({450.f, 300.f});
    menuBackground.setFillColor(sf::Color(50, 50, 50, 220));
    menuBackground.setOutlineThickness(3.f);
    menuBackground.setOutlineColor(sf::Color::White);

    // Quit button
    quitButton.setSize({100.f, 40.f});
    quitButton.setPosition({500.f, 380.f});
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setOutlineThickness(2.f);
    quitButton.setOutlineColor(sf::Color::White);

    // Cancel button
    cancelButton.setSize({100.f, 40.f});
    cancelButton.setPosition({620.f, 380.f});
    cancelButton.setFillColor(sf::Color::Green);
    cancelButton.setOutlineThickness(2.f);
    cancelButton.setOutlineColor(sf::Color::White);
}

void Menu::handleEvent(const sf::Event &event, sf::RenderWindow &window)
{
    if (!visible)
        return;

    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos{(float)clicked->position.x, (float)clicked->position.y};

            if (quitButton.getGlobalBounds().contains(mousePos))
            {
                quit = true;
            }
            else if (cancelButton.getGlobalBounds().contains(mousePos))
            {
                visible = false;
            }
        }
    }
}

void Menu::draw(sf::RenderWindow &window)
{
    if (!visible)
        return;

    // Save current view
    sf::View currentView = window.getView();
    sf::View menuView({600.f, 400.f}, {1200.f, 800.f});
    window.setView(menuView);

    // Draw menu
    window.draw(menuBackground);
    window.draw(quitButton);
    window.draw(cancelButton);

    // Draw text labels
    drawSimpleText(window, "Digital Logic Simulator", sf::Vector2f(480.f, 320.f), sf::Color::White);
    drawSimpleText(window, "Do you want to quit?", sf::Vector2f(490.f, 350.f), sf::Color::White);
    drawSimpleText(window, "QUIT", sf::Vector2f(520.f, 395.f), sf::Color::White);
    drawSimpleText(window, "CANCEL", sf::Vector2f(635.f, 395.f), sf::Color::White);

    // Restore view
    window.setView(currentView);
}

void Menu::drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color)
{
    // Draw simple pixel-based text using rectangles
    for (size_t i = 0; i < text.length(); ++i)
    {
        if (text[i] != ' ')
        {
            sf::RectangleShape charRect({6.f, 8.f});
            charRect.setPosition({position.x + i * 7.f, position.y});
            charRect.setFillColor(color);
            window.draw(charRect);
        }
    }
}
