#pragma once
#include <SFML/Graphics.hpp>

class WindowConfig {
   public:
    static constexpr float SCREEN_SCALE = 1.f;     // 80% of screen
    static constexpr float PALETTE_SCALE = 0.16f;  // 16% of window
    static constexpr float CANVAS_SCALE = 0.84f;   // 84% of window

    static sf::Vector2f getWindowSize() {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        return sf::Vector2f(desktop.size.x * SCREEN_SCALE, desktop.size.y * SCREEN_SCALE);
    }

    static sf::Vector2f getPaletteSize() {
        sf::Vector2f windowSize = getWindowSize();
        return sf::Vector2f(windowSize.x * PALETTE_SCALE, windowSize.y);
    }

    static sf::Vector2f getCanvasSize() {
        sf::Vector2f windowSize = getWindowSize();
        return sf::Vector2f(windowSize.x * CANVAS_SCALE, windowSize.y);
    }
};
