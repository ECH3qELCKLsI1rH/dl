// ui/Canvas.h

#pragma once
#include <SFML/Graphics.hpp>

#include "../engine/Simulator.h"

class Canvas {
   private:
    sf::View view;
    sf::Vector2f initialPos{};
    bool dragging = false;
    Simulator &simulator;

    void drawGrid(sf::RenderWindow &window);

   public:
    explicit Canvas(Simulator &simulator);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    void update();
    void draw(sf::RenderWindow &window);
    const sf::View &getView() const { return view; }
};
