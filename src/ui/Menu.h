#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Menu
{
public:
    Menu();

    void handleEvent(const sf::Event &event, sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void setFont(const sf::Font &font);

    bool isVisible() const { return visible; }
    void setVisible(bool vis) { visible = vis; }
    bool shouldQuit() const { return quit; }

private:
    bool visible = false;
    bool quit = false;
    bool quitButtonHovered = false;
    bool cancelButtonHovered = false;

    sf::RectangleShape menuBackground;
    sf::RectangleShape quitButton;
    sf::RectangleShape cancelButton;

    std::optional<sf::Text> titleText;
    std::optional<sf::Text> questionText;
    std::optional<sf::Text> quitButtonText;
    std::optional<sf::Text> cancelButtonText;
    const sf::Font *currentFont = nullptr;

    void setupMenu();
    void setupTexts();
};
