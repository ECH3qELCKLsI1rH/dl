#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    Menu();

    void handleEvent(const sf::Event &event, sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);

    bool isVisible() const { return visible; }
    void setVisible(bool vis) { visible = vis; }
    bool shouldQuit() const { return quit; }

private:
    bool visible = false;
    bool quit = false;

    sf::RectangleShape menuBackground;
    sf::RectangleShape quitButton;
    sf::RectangleShape cancelButton;

    void setupMenu();
    void drawSimpleText(sf::RenderWindow &window, const std::string &text, sf::Vector2f position, sf::Color color);
};
