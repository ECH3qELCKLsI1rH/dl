#include <SFML/Graphics.hpp>
#include <optional>
#include "ui/Canvas.h"
#include "ui/ComponentPalette.h"
#include "ui/Menu.h"
#include "engine/Simulator.h"

#include <iostream>
#include <string>

void LoadFont(sf::Font &font, std::string str)
{
    if (!font.openFromFile(str))
    {
        std::cerr << "Error: Could not load font file " << str << "!!!" << std::endl;
    }
}

int main()
{
    // Get the current desktop resolution
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Use 80% of the current screen size for windowed mode
    unsigned int width = static_cast<unsigned int>(desktop.size.x * 0.8f);
    unsigned int height = static_cast<unsigned int>(desktop.size.y * 0.8f);

    sf::RenderWindow window(
        sf::VideoMode({width, height}),
        "Digital Logic Simulator - Logic Expression & Truth Table Generator",
        sf::Style::Default);

    // Center the window on screen
    window.setPosition({static_cast<int>((desktop.size.x - width) / 2),
                        static_cast<int>((desktop.size.y - height) / 2)});

    // Create a single simulator instance shared by all components
    Simulator simulator;
    Canvas canvas(simulator);
    ComponentPalette palette;
    Menu menu;

    // Fonts
    sf::Font font;
    LoadFont(font, "assets/fonts/arial.ttf");

    // Set font for all UI components
    simulator.setFont(font);
    palette.setFont(font);
    menu.setFont(font);

    sf::Text text(font);
    text.setString("Test text");
    text.setOrigin(text.getGlobalBounds().size / 2.f);
    text.setPosition({width / 2.f, height / 2.f});
    text.setFillColor(sf::Color::Green);
    text.setOutlineThickness(2.f);
    text.setOutlineColor(sf::Color::Blue);

    sf::Clock clock;

    // Cursor management for SFML 3.0
    bool isHandCursor = false;

    float dt = clock.restart().asSeconds();
    std::stringstream sStream;
    sStream << "Time: " << dt;

    text.setString(sStream.str());

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
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
                {
                    window.close();
                    break;
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

            // Check for mouse hover over clickable items to change cursor
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos{(float)mousePixel.x, (float)mousePixel.y};
            bool shouldShowHand = false;

            // Check if mouse is over menu buttons when visible
            if (menu.isVisible())
            {
                // Menu uses a specific view, need to transform coordinates
                sf::View menuView({600.f, 400.f}, {1200.f, 800.f});
                sf::Vector2f transformedPos = window.mapPixelToCoords(mousePixel, menuView);

                // Check quit button (500, 380, 100x40)
                if (transformedPos.x >= 500.f && transformedPos.x <= 600.f &&
                    transformedPos.y >= 380.f && transformedPos.y <= 420.f)
                {
                    shouldShowHand = true;
                }
                // Check cancel button (620, 380, 100x40)
                else if (transformedPos.x >= 620.f && transformedPos.x <= 720.f &&
                         transformedPos.y >= 380.f && transformedPos.y <= 420.f)
                {
                    shouldShowHand = true;
                }
            }
            // Check if mouse is over palette buttons when menu is not visible
            else if (mousePos.x <= 120.f)
            {
                // Palette area - check for buttons (20, 50+i*50, 80x40)
                for (int i = 0; i < 5; ++i) // 5 gate types
                {
                    if (mousePos.x >= 20.f && mousePos.x <= 100.f &&
                        mousePos.y >= (50.f + i * 50.f) && mousePos.y <= (90.f + i * 50.f))
                    {
                        shouldShowHand = true;
                        break;
                    }
                }
            }

            // Update cursor based on hover state
            if (shouldShowHand && !isHandCursor)
            {
                // Change to hand cursor (simulated by changing window title for feedback)
                isHandCursor = true;
            }
            else if (!shouldShowHand && isHandCursor)
            {
                // Change back to arrow cursor
                isHandCursor = false;
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
        // canvas.draw(window);
        palette.draw(window);
        simulator.drawUI(window);

        // Draw menu last (on top of everything)
        menu.draw(window);

        window.draw(text);

        window.display();
    }

    return 0;
}