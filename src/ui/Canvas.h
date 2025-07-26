#pragma once
#include <SFML/Graphics.hpp>
#include "../engine/Simulator.h"

class Canvas
{
public:
    Canvas();

    void handleEvent(const sf::Event &event);
    void update();
    void draw(sf::RenderWindow &window);

    sf::View &getView() { return view; }
    void setView(const sf::View &v) { view = v; }

private:
    sf::View view;
    sf::Vector2f lastMousePos{};
    bool dragging = false;
    Simulator simulator;

    void drawGrid(sf::RenderWindow &window);
};