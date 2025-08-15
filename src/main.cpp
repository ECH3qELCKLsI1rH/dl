// main.cpp

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

#include "engine/Simulator.h"
#include "ui/Canvas.h"
#include "ui/ComponentPalette.h"
#include "ui/Configuration.h"

void LoadFont(sf::Font &font, std::string str) {
    if (!font.openFromFile(str)) {
        std::cerr << "Error: Could not load font file " << str << "!!!" << std::endl;
    }
}

int main() {
    // Window setup
    sf::Vector2f windowSize = WindowConfig::getWindowSize();
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(windowSize.x), static_cast<unsigned int>(windowSize.y)}), "Digital Logic Suite",
                            sf::Style::Default);
    window.setPosition({static_cast<int>((desktop.size.x - windowSize.x) / 2), static_cast<int>((desktop.size.y - windowSize.y) / 2)});

    // Core systems
    Simulator simulator;
    Canvas canvas(simulator);
    ComponentPalette palette;

    // Load font
    sf::Font font;
    LoadFont(font, "src/assets/fonts/arial.ttf");
    simulator.setFont(font);
    palette.setFont(font);

    bool isHandCursor = false;

    // Main loop
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->scancode) {
                    case sf::Keyboard::Scancode::Escape:
                        simulator.cancelSelection();
                        break;
                    case sf::Keyboard::Scancode::T:
                        simulator.generateTruthTable();
                        break;
                    case sf::Keyboard::Scancode::E:
                        simulator.generateLogicalExpression();
                        break;
                    case sf::Keyboard::Scancode::C:
                        simulator.clearCircuit();
                        break;
                    case sf::Keyboard::Scancode::Delete:
                        simulator.deleteSelectedGates();
                        break;
                    case sf::Keyboard::Scancode::I:
                        simulator.toggleInputField();
                        break;
                    case sf::Keyboard::Scancode::Q:
                        window.close();
                        break;
                    default:
                        break;
                }
            }

            // Hand cursor detection for palette
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos{static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y)};
            bool shouldShowHand = false;

            if (mousePos.x <= 120.f) {
                for (int i = 0; i < 5; ++i) {
                    if (mousePos.x >= 20.f && mousePos.x <= 100.f && mousePos.y >= (50.f + i * 50.f) && mousePos.y <= (90.f + i * 50.f)) {
                        shouldShowHand = true;
                        break;
                    }
                }
            }

            if (shouldShowHand != isHandCursor) {
                isHandCursor = shouldShowHand;
            }

            // Handle events for core UI
            canvas.handleEvent(*event, window);
            palette.handleEvent(*event, window);
            simulator.handleEvent(*event, window, canvas.getView(), palette.getSelectedGateType());
        }

        // Update systems
        canvas.update();
        palette.update();
        simulator.update();

        // Draw
        window.clear(sf::Color(30, 30, 30));
        canvas.draw(window);
        palette.draw(window);
        simulator.drawUI(window);
        window.display();
    }

    return 0;
}
