// ui/Configuration.h

#pragma once
#include <SFML/Graphics.hpp>

class WindowConfig {
   public:
    static constexpr float PALETTE_SCALE = 0.15f;  // 15% of window width

    static sf::Vector2f getWindowSize() {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        return sf::Vector2f(desktop.size.x, desktop.size.y);
    }

    static sf::Vector2f getPaletteSize() {
        sf::Vector2f windowSize = getWindowSize();
        return sf::Vector2f(windowSize.x * PALETTE_SCALE, windowSize.y);
    }

    static sf::Vector2f getCanvasSize() {
        sf::Vector2f windowSize = getWindowSize();
        return sf::Vector2f(windowSize.x, windowSize.y);
    }
};
