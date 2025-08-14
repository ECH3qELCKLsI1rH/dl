#include "Menu.h"

Menu::Menu() { setupMenu(); }  // call setupMenu() in constructor

void Menu::setupMenu() {
    // menuBackground, quitButton, cancelButton are RectangleShape
    menuBackground.setSize({300.f, 200.f});
    menuBackground.setPosition({450.f, 300.f});
    menuBackground.setFillColor(sf::Color(50, 50, 50, 220));
    menuBackground.setOutlineThickness(3.f);
    menuBackground.setOutlineColor(sf::Color::White);

    quitButton.setSize({100.f, 40.f});
    quitButton.setPosition({500.f, 380.f});
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setOutlineThickness(2.f);
    quitButton.setOutlineColor(sf::Color::White);

    cancelButton.setSize({100.f, 40.f});
    cancelButton.setPosition({620.f, 380.f});
    cancelButton.setFillColor(sf::Color::Green);
    cancelButton.setOutlineThickness(2.f);
    cancelButton.setOutlineColor(sf::Color::White);

    if (currentFont) {
        setupTexts();
    }
}

void Menu::setFont(const sf::Font &font) {
    currentFont = &font;  // store font pointer to call from main.cpp
    setupTexts();
}

void Menu::setupTexts() {
    if (currentFont == nullptr) return;  // skip if no font

    titleText = sf::Text(*currentFont);               // title text
    titleText->setString("Digital Logic Simulator");  // text content
    titleText->setCharacterSize(18);                  // font size
    titleText->setFillColor(sf::Color::White);        // text color
    titleText->setPosition({480.f, 320.f});           // pos

    questionText = sf::Text(*currentFont);            // question text
    questionText->setString("Do you want to quit?");  // content
    questionText->setCharacterSize(14);               // font size
    questionText->setFillColor(sf::Color::White);     // color
    questionText->setPosition({490.f, 350.f});        // pos

    quitButtonText = sf::Text(*currentFont);                                                      // quit btn label
    quitButtonText->setString("QUIT");                                                            // text
    quitButtonText->setCharacterSize(14);                                                         // size
    quitButtonText->setFillColor(sf::Color::White);                                               // color
    sf::FloatRect quitBounds = quitButtonText->getLocalBounds();                                  // get bounds
    sf::Vector2f quitButtonPos = quitButton.getPosition();                                        // btn pos
    sf::Vector2f quitButtonSize = quitButton.getSize();                                           // btn size
    quitButtonText->setPosition({quitButtonPos.x + (quitButtonSize.x - quitBounds.size.x) / 2,    // center X
                                 quitButtonPos.y + (quitButtonSize.y - quitBounds.size.y) / 2});  // center Y

    cancelButtonText = sf::Text(*currentFont);                                                            // cancel btn label
    cancelButtonText->setString("CANCEL");                                                                // text
    cancelButtonText->setCharacterSize(14);                                                               // size
    cancelButtonText->setFillColor(sf::Color::White);                                                     // color
    sf::FloatRect cancelBounds = cancelButtonText->getLocalBounds();                                      // get bounds
    sf::Vector2f cancelButtonPos = cancelButton.getPosition();                                            // btn pos
    sf::Vector2f cancelButtonSize = cancelButton.getSize();                                               // btn size
    cancelButtonText->setPosition({cancelButtonPos.x + (cancelButtonSize.x - cancelBounds.size.x) / 2,    // center X
                                   cancelButtonPos.y + (cancelButtonSize.y - cancelBounds.size.y) / 2});  // center Y
}

void Menu::handleEvent(const sf::Event &event, sf::RenderWindow &window) {
    if (!visible) return;  // skip if menu hidden

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);               // mouse in px
    sf::View menuView({600.f, 400.f}, {1200.f, 800.f});                     // fixed menu view
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, menuView);  // map to coords

    quitButtonHovered = quitButton.getGlobalBounds().contains(mousePos);      // hover check quit
    cancelButtonHovered = cancelButton.getGlobalBounds().contains(mousePos);  // hover check cancel

    if (quitButtonHovered) {                              // if over quit
        quitButton.setFillColor(sf::Color(200, 50, 50));  // lighter red
    } else {
        quitButton.setFillColor(sf::Color::Red);  // normal red
    }

    if (cancelButtonHovered) {                              // if over cancel
        cancelButton.setFillColor(sf::Color(50, 200, 50));  // lighter green
    } else {
        cancelButton.setFillColor(sf::Color::Green);  // normal green
    }

    if (const auto *clicked = event.getIf<sf::Event::MouseButtonPressed>()) {  // mouse click
        if (clicked->button == sf::Mouse::Button::Left) {                      // left click
            if (quitButton.getGlobalBounds().contains(mousePos)) {             // click quit
                quit = true;                                                   // mark quit
            } else if (cancelButton.getGlobalBounds().contains(mousePos)) {    // click cancel
                visible = false;                                               // hide menu
            }
        }
    }
}

void Menu::draw(sf::RenderWindow &window) {
    if (!visible) return;  // skip if hidden

    sf::View currentView = window.getView();             // save view
    sf::View menuView({600.f, 400.f}, {1200.f, 800.f});  // fixed menu view
    window.setView(menuView);                            // use menu view

    window.draw(menuBackground);  // bg
    window.draw(quitButton);      // quit btn
    window.draw(cancelButton);    // cancel btn

    if (currentFont != nullptr) {                                                 // if font
        if (titleText.has_value()) window.draw(titleText.value());                // draw title
        if (questionText.has_value()) window.draw(questionText.value());          // draw question
        if (quitButtonText.has_value()) window.draw(quitButtonText.value());      // draw quit label
        if (cancelButtonText.has_value()) window.draw(cancelButtonText.value());  // draw cancel label
    }

    window.setView(currentView);  // restore view
}
