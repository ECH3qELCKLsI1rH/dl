cmake_minimum_required(VERSION 3.10)
project(DigitalLogicSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(digital-logic-sim ${SOURCES})

target_link_libraries(digital-logic-sim
    SFML::Graphics
    SFML::Window
    SFML::System
)

target_include_directories(digital-logic-sim PRIVATE src)
