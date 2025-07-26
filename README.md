# Digital Logic Simulator - Logic Expression & Truth Table Generator

A powerful digital logic simulator built with SFML 3.0 that allows you to create logic circuits, generate logical expressions, and produce truth tables.

## Features

- **Interactive Circuit Design**: Drag and drop logic gates to create complex circuits
- **Real-time Simulation**: Watch your circuit respond to input changes in real-time
- **Logic Expression Generation**: Automatically generate mathematical expressions from your circuits
- **Truth Table Generation**: Create comprehensive truth tables for your logic circuits
- **Visual Wire Connections**: Connect gates with colored wires that show signal flow

## Supported Logic Gates

- **INPUT**: Toggle-able input switches (Blue)
- **AND**: Logic AND gate (Green)
- **OR**: Logic OR gate (Cyan)
- **NOT**: Logic NOT gate (Magenta)
- **OUTPUT**: Output indicator (Red)

## Controls

### Mouse Controls

- **Left Click**:
  - On palette: Select gate type
  - On canvas: Place selected gate
  - On INPUT gate: Toggle state (on/off)
  - On gate pins: Start/complete wire connections
- **Right Click**: Cancel wire placement
- **Middle Click + Drag**: Pan around the canvas
- **Mouse Wheel**: Zoom in/out

### Keyboard Controls

- **T**: Generate and display Truth Table
- **E**: Generate and display Logical Expression
- **C**: Clear the entire circuit
- **Escape**: Exit the application

## How to Use

### Building a Circuit

1. **Select a Component**: Click on a gate type in the left panel (INPUT, AND, OR, NOT, OUTPUT)
2. **Place Gates**: Click anywhere on the canvas to place the selected gate
3. **Connect Gates**:
   - Click on an output pin (right side of gate)
   - Then click on an input pin (left side of target gate)
   - Wires will automatically connect the gates

### Testing Your Circuit

1. **Toggle Inputs**: Click on INPUT gates (blue) to change their state between 0 and 1
2. **Observe Outputs**: Watch how the circuit responds in real-time
3. **View Connections**: Wires change color based on signal state

### Generating Analysis

1. **Truth Table**: Press **T** to generate a complete truth table showing all input combinations and their corresponding outputs
2. **Logical Expression**: Press **E** to generate the mathematical boolean expression for your circuit
3. **Clear Circuit**: Press **C** to start over with a fresh canvas

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- SFML 3.0 libraries

### Compilation

```bash
# Configure the project
cmake -S . -B build

# Build the project
cmake --build build

# Run the simulator
./build/digital-logic-sim
```

## Example Circuits

### Simple AND Gate

1. Place 2 INPUT gates
2. Place 1 AND gate
3. Place 1 OUTPUT gate
4. Connect INPUT outputs to AND inputs
5. Connect AND output to OUTPUT input
6. Toggle inputs and press T for truth table

### XOR Implementation (using AND, OR, NOT)

Create an XOR gate using basic gates:

- XOR = (A ∧ ¬B) ∨ (¬A ∧ B)

## Architecture

The simulator is built with a modular design:

- **Engine**: Core logic simulation (`Gate.cpp`, `Wire.cpp`, `Simulator.cpp`)
- **UI**: User interface components (`Canvas.cpp`, `ComponentPalette.cpp`)
- **Main**: Application entry point and event handling

## Contributing

This project demonstrates:

- Modern C++17 features
- SFML 3.0 graphics and event handling
- Clean architecture with separation of concerns
- Real-time simulation algorithms
- Expression parsing and truth table generation

Feel free to extend the simulator with additional gate types (XOR, NAND, NOR) or features like circuit saving/loading!
