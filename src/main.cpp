#include <SFML/Graphics.hpp>
#include <optional>
#include "ui/Canvas.h"
#include "ui/ComponentPalette.h"
#include "ui/Menu.h"
#include "engine/Simulator.h"

int main()
{
    unsigned int width = 1200;
    unsigned int height = 800;
    sf::RenderWindow window(sf::VideoMode({width, height}), "Digital Logic Simulator - Logic Expression & Truth Table Generator");

    // Create a single simulator instance shared by all components
    Simulator simulator;
    Canvas canvas(simulator);
    ComponentPalette palette;
    Menu menu;

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
                    // Cancel selection instead of quitting
                    simulator.cancelSelection();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::T)
                {
                    simulator.generateTruthTable();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::E)
                {
                    simulator.generateLogicalExpression();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::C)
                {
                    simulator.clearCircuit();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Delete)
                {
                    simulator.deleteSelectedGates();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::F)
                {
                    menu.setVisible(!menu.isVisible());
                }
            }

            // Handle menu events first (highest priority)
            menu.handleEvent(*event, window);

            // If menu wants to quit, close the window
            if (menu.shouldQuit())
            {
                window.close();
                break;
            }

            // Only handle other events if menu is not visible
            if (!menu.isVisible())
            {
                // Forward events to components
                canvas.handleEvent(*event, window);
                palette.handleEvent(*event, window);
                simulator.handleEvent(*event, window, canvas.getView(), palette.getSelectedGateType());
            }
        }

        canvas.update();
        palette.update();
        simulator.update();

        window.clear(sf::Color(30, 30, 30));
        canvas.draw(window);
        palette.draw(window);
        simulator.drawUI(window);

        // Draw menu last (on top of everything)
        menu.draw(window);

        window.display();
    }

    return 0;
}