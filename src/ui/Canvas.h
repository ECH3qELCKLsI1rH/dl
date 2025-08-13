#pragma once
#include <SFML/Graphics.hpp>

#include "../engine/Simulator.h"

class Canvas {
   private:
    sf::View view;

    // sf::Vector2f holds two float values: x and y
    // {} ensures that when the program starts, initialPos.x = 0.0f and initialPos.y = 0.0f, instead of leaving them with garbage values
    sf::Vector2f initialPos{};

    bool dragging = false;
    Simulator &simulator;
    void drawGrid(sf::RenderWindow &window);

   public:
    Canvas(Simulator &simulator);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);
    const sf::View &getView() const { return view; }
};