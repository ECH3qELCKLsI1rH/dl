#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "engine/Simulator.h"
#include "ui/Canvas.h"
#include "ui/ComponentPalette.h"
#include "ui/Menu.h"

void LoadFont(sf::Font &font, std::string str) {
    if (!font.openFromFile(str)) {
        std::cerr << "Error: Could not load font file " << str << "!!!" << std::endl;
    }
}

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int width = static_cast<unsigned int>(desktop.size.x * 0.8f);
    unsigned int height = static_cast<unsigned int>(desktop.size.y * 0.8f);

    sf::RenderWindow window(sf::VideoMode({width, height}), "Digital Logic Simulator - Logic Expression & Truth Table Generator", sf::Style::Default);

    window.setPosition({static_cast<int>((desktop.size.x - width) / 2), static_cast<int>((desktop.size.y - height) / 2)});

    Simulator simulator;
    Canvas canvas(simulator);
    ComponentPalette palette;
    Menu menu;

    sf::Font font;
    LoadFont(font, "assets/fonts/arial.ttf");

    simulator.setFont(font);
    palette.setFont(font);
    menu.setFont(font);

    bool isHandCursor = false;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    simulator.cancelSelection();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::T) {
                    simulator.generateTruthTable();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::E) {
                    simulator.generateLogicalExpression();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::C) {
                    simulator.clearCircuit();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Delete) {
                    simulator.deleteSelectedGates();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::F) {
                    menu.setVisible(!menu.isVisible());
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::I) {
                    simulator.toggleInputField();
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
                    window.close();
                    break;
                }
            }

            menu.handleEvent(*event, window);

            if (menu.shouldQuit()) {
                window.close();
                break;
            }

            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos{(float)mousePixel.x, (float)mousePixel.y};
            bool shouldShowHand = false;

            if (menu.isVisible()) {
                sf::View menuView({600.f, 400.f}, {1200.f, 800.f});
                sf::Vector2f transformedPos = window.mapPixelToCoords(mousePixel, menuView);

                if (transformedPos.x >= 500.f && transformedPos.x <= 600.f && transformedPos.y >= 380.f && transformedPos.y <= 420.f) {
                    shouldShowHand = true;
                } else if (transformedPos.x >= 620.f && transformedPos.x <= 720.f && transformedPos.y >= 380.f && transformedPos.y <= 420.f) {
                    shouldShowHand = true;
                }
            } else if (mousePos.x <= 120.f) {
                for (int i = 0; i < 5; ++i) {
                    if (mousePos.x >= 20.f && mousePos.x <= 100.f && mousePos.y >= (50.f + i * 50.f) && mousePos.y <= (90.f + i * 50.f)) {
                        shouldShowHand = true;
                        break;
                    }
                }
            }

            if (shouldShowHand && !isHandCursor) {
                isHandCursor = true;
            } else if (!shouldShowHand && isHandCursor) {
                isHandCursor = false;
            }

            if (!menu.isVisible()) {
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

        menu.draw(window);

        window.display();
    }

    return 0;
}