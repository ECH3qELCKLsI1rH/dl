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

    // Setup texts if font is available
    if (currentFont != nullptr)
    {
        setupTexts();
    }
}

void Menu::setFont(const sf::Font &font)
{
    currentFont = &font;
    setupTexts();
}

void Menu::setupTexts()
{
    if (currentFont == nullptr)
        return;

    // Title text
    titleText = sf::Text(*currentFont);
    titleText->setString("Digital Logic Simulator");
    titleText->setCharacterSize(18);
    titleText->setFillColor(sf::Color::White);
    titleText->setPosition({480.f, 320.f});

    // Question text
    questionText = sf::Text(*currentFont);
    questionText->setString("Do you want to quit?");
    questionText->setCharacterSize(14);
    questionText->setFillColor(sf::Color::White);
    questionText->setPosition({490.f, 350.f});

    // Quit button text
    quitButtonText = sf::Text(*currentFont);
    quitButtonText->setString("QUIT");
    quitButtonText->setCharacterSize(14);
    quitButtonText->setFillColor(sf::Color::White);
    sf::FloatRect quitBounds = quitButtonText->getLocalBounds();
    sf::Vector2f quitButtonPos = quitButton.getPosition();
    sf::Vector2f quitButtonSize = quitButton.getSize();
    quitButtonText->setPosition({quitButtonPos.x + (quitButtonSize.x - quitBounds.size.x) / 2,
                                 quitButtonPos.y + (quitButtonSize.y - quitBounds.size.y) / 2});

    // Cancel button text
    cancelButtonText = sf::Text(*currentFont);
    cancelButtonText->setString("CANCEL");
    cancelButtonText->setCharacterSize(14);
    cancelButtonText->setFillColor(sf::Color::White);
    sf::FloatRect cancelBounds = cancelButtonText->getLocalBounds();
    sf::Vector2f cancelButtonPos = cancelButton.getPosition();
    sf::Vector2f cancelButtonSize = cancelButton.getSize();
    cancelButtonText->setPosition({cancelButtonPos.x + (cancelButtonSize.x - cancelBounds.size.x) / 2,
                                   cancelButtonPos.y + (cancelButtonSize.y - cancelBounds.size.y) / 2});
}

void Menu::handleEvent(const sf::Event &event, sf::RenderWindow &window)
{
    if (!visible)
        return;

    // Get mouse position for hover detection
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::View menuView({600.f, 400.f}, {1200.f, 800.f});
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, menuView);

    // Update hover states
    quitButtonHovered = quitButton.getGlobalBounds().contains(mousePos);
    cancelButtonHovered = cancelButton.getGlobalBounds().contains(mousePos);

    // Update button colors based on hover state
    if (quitButtonHovered)
    {
        quitButton.setFillColor(sf::Color(200, 50, 50)); // Lighter red on hover
    }
    else
    {
        quitButton.setFillColor(sf::Color::Red); // Normal red
    }

    if (cancelButtonHovered)
    {
        cancelButton.setFillColor(sf::Color(50, 200, 50)); // Lighter green on hover
    }
    else
    {
        cancelButton.setFillColor(sf::Color::Green); // Normal green
    }

    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (clicked->button == sf::Mouse::Button::Left)
        {
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

    // Draw text labels using proper text
    if (currentFont != nullptr)
    {
        if (titleText.has_value())
            window.draw(titleText.value());
        if (questionText.has_value())
            window.draw(questionText.value());
        if (quitButtonText.has_value())
            window.draw(quitButtonText.value());
        if (cancelButtonText.has_value())
            window.draw(cancelButtonText.value());
    }

    // Restore view
    window.setView(currentView);
}
