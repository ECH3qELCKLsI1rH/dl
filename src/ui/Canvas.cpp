#include "Canvas.h"

#include <cmath>

Canvas::Canvas(Simulator &simulator) : simulator(simulator) {
    /* The canvas is much bigger than the window’s display area.
        The view defines:
        Center → The coordinates in the canvas where the camera points.
        Size → The width and height of the visible area*/
    view.setCenter({600.f, 400.f});  // set the center point of the view to 600px,400px coordinates
    view.setSize({1200.f, 800.f});   // set the size of the view to (1200x800)px
}

void Canvas::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    // event.getIf<some event> returns a pointer to that specific event structure, otherwise nullptr
    if (const auto *wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {  // if event is a mouse wheel scroll
        float factor = (wheel->delta > 0) ? 0.9f : 1.1f;  // delta = +ve = scroll up (zoom by shrinking the view) else: opposite garne
        view.setSize(view.getSize() * factor);            // update view ko size
    }

    if (const auto *pressed = event.getIf<sf::Event::MouseButtonPressed>()) {  // if event is a mouse button press
        if (pressed->button == sf::Mouse::Button::Middle) {                    // where, the button is the mouse middle button,
            dragging = true;                                                   // set "dragging" state to true,
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);          // get the position of the mouse,
            initialPos = sf::Vector2f{(float)mousePixel.x, (float)mousePixel.y};
        }
    }

    if (const auto *released = event.getIf<sf::Event::MouseButtonReleased>()) {  // if the event is a mouse button release
        if (released->button == sf::Mouse::Button::Middle) {                     // where, the button is the mouse middle button
            dragging = false;                                                    // set "dragginf" state to false
        }
    }

    if (const auto *moved = event.getIf<sf::Event::MouseMoved>()) {    // if the event is the mouse moving,
        if (dragging) {                                                // while the middle mouse is being pressed
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);  // get the position of the mouse
            sf::Vector2f finalPos{(float)mousePixel.x, (float)mousePixel.y};
            sf::Vector2f difference = initialPos - finalPos;           // calculate how mush the mouse has moved
            float zoomFactor = view.getSize().x / window.getSize().x;  // to make sure that zooming in/out doesnt affect panning
            difference *= zoomFactor;                                  // update the difference based on the zoom level of the canvas
            view.move(difference);                                     // move the camera by "difference" amount
            initialPos = finalPos;                                     // update the mouse position
        }
    }
}

void Canvas::update() { simulator.update(); }  // simply calls simulator ko update()

void Canvas::drawGrid(sf::RenderWindow &window) {
    const float gridSize = 50.f;             // to draw the grid layout (length = breadth = 50px)
    sf::Vector2f size = view.getSize();      // get the size of the view (visible area)
    sf::Vector2f center = view.getCenter();  // get tht center coords of the view (visible area)

    float left = center.x - size.x / 2.f;    // find the left boundary ko coordinate of the visible region
    float right = center.x + size.x / 2.f;   //          right
    float top = center.y - size.y / 2.f;     //          top
    float bottom = center.y + size.y / 2.f;  //          bottom

    sf::VertexArray lines(sf::PrimitiveType::Lines);
    // Wire.cpp ko jastai but no vertices (for loop bhitra append garne ( also the color of the line in the 3rd argument))

    for (float x = std::floor(left / gridSize) * gridSize; x <= right; x += gridSize) {  // to draw vertical lines
        lines.append({{x, top}, sf::Color(60, 60, 60)});                                 // for each x, adds two vertices to the top,
        lines.append({{x, bottom}, sf::Color(60, 60, 60)});                              // and the bottom
    }
    for (float y = std::floor(top / gridSize) * gridSize; y <= bottom; y += gridSize) {  // same thing but for horz lines
        lines.append({{left, y}, sf::Color(60, 60, 60)});
        lines.append({{right, y}, sf::Color(60, 60, 60)});
    }
    window.draw(lines);  // finally, draw the lines
}

void Canvas::draw(sf::RenderWindow &window) {
    window.setView(view);    // sets the camera of the window to "view"
    drawGrid(window);        // draws the grid layout
    simulator.draw(window);  // draw all simulation elements on the canvas
}