TARGET = main
CC = clang++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS = -Wwritable-strings -std=c++17 -I/usr/local/include -I/usr/include/SDL2 -I./lib/imgui/include/
SRC=./src
LIB=./lib
BIN=./bin

.PHONY: default all clean

OBJECTS = $(patsubst $(SRC)/%.cc, $(BIN)/%.o, $(wildcard $(SRC)/*.cc) $(wildcard $(SRC)/**/*.cc) $(LIB)/pugixml-1.10/src/pugixml.cpp $(wildcard $(LIB)/imgui/src/*.cpp))
HEADERS = $(wildcard $(SRC)*.h lib/pugixml-1.0/src/pugixml.hpp)

default: clean $(TARGET) copyassets #run
all: default

$(BIN)/%.o: $(SRC)/%.cc $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN)/$@

copyassets: ./assets
		-cp -r assets/ $(BIN)/assets

clean:
		-rm -rf $(BIN)/*
		-mkdir $(BIN)/components


#.PHONY: run
#run: $(TARGET)
#		$(BIN)/$(TARGET)
