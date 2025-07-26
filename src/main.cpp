#include <SFML/Graphics.hpp>
#include <optional>
#include "ui/Canvas.h"
#include "ui/ComponentPalette.h"
#include "engine/Simulator.h"

int main()
{
    unsigned int width = 800;
    unsigned int height = 600;
    sf::RenderWindow window(sf::VideoMode({width, height}), "Digital Logic Simulator");

    Canvas canvas;
    ComponentPalette palette;
    Simulator simulator;

    // Initialize 2 input switches for testing
    // simulator.addInput();
    // simulator.addInput();

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }
            }

            // Forward events to canvas, palette, and simulator
            canvas.handleEvent(*event);
            palette.handleEvent(*event);
            simulator.handleEvent(*event, window, canvas.view, palette.getSelectedGateType());
        }

        canvas.update();
        palette.update();
        simulator.update();

        window.clear(sf::Color::Black);
        canvas.draw(window);
        palette.draw(window);
        window.display();
    }

    return 0;
}