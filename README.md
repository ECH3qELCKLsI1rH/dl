Digital Logic Simulator
A simple digital logic simulator built with SFML 3.0, allowing users to create and simulate basic logic circuits.
Prerequisites

CMake 3.10 or higher
SFML 3.0
C++17 compatible compiler (e.g., GCC 9, MSVC 16, Clang 9)

Build Instructions
mkdir build && cd build
cmake ..
make
./digital-logic-sim

Usage

Use the mouse wheel to zoom the canvas.
Middle-click and drag to pan the canvas.
Select logic gates from the component palette (WIP).
Simulate basic logic circuits (WIP).

Folder Structure

assets/fonts/: Font files for UI text.
assets/icons/: Icon images for gates.
src/: Source code.
engine/: Logic for gates, wires, and simulation.
ui/: UI components like canvas and palette.


