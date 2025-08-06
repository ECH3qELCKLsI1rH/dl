TARGET = program.exe
SRC = src/main.cpp src/ui/*.cpp src/engine/*.cpp
INCLUDE = -I"src/SFML-3.0.0/include"
LIBRARY = -L"src/SFML-3.0.0/lib"
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32

all:
	g++ -std=c++17 $(SRC) -o $(TARGET) $(INCLUDE) -DSFML_STATIC $(LIBRARY) $(LIBS)

run: all
	$(TARGET)

clean:
	del /Q $(TARGET)

# "make" 		: compile matra
# "make run" 	: compile and run
# "make clean" 	: remove generated .exe file