#pragma once
#include <SFML/Graphics.hpp>

#include "../engine/Simulator.h"

class Canvas {
   public:
    Canvas(Simulator &simulator);

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);

    const sf::View &getView() const { return view; }

   private:
    sf::View view;
    sf::Vector2f lastMousePos{};
    bool dragging = false;
    Simulator &simulator;

    void drawGrid(sf::RenderWindow &window);
};