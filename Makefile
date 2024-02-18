TARGET = main
#CC = clang++
CC = g++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image
#CFLAGS = -Wwritable-strings -std=c++17 -I/usr/local/include -I/usr/include/SDL2 -I./lib/imgui/include/
OPT=-O0
INCDIRS=-I/usr/include/SDL2 -I./lib/pugixml-1.10/src/ -I./lib/imgui/include/
CFLAGS = -Wwrite-strings -std=c++17 -I/usr/local/include $(INCDIRS) -MMD -MP $(OPT)
SRC=./src
LIB=./lib
BIN=./bin

.PHONY: default all clean

SOURCES = $(wildcard $(SRC)/*.cc)

OBJECTS = $(patsubst $(SRC)/%.cc, $(BIN)/%.o, $(SOURCES))
#$(LIB)/pugixml-1.10/src/pugixml.cpp $(wildcard $(LIB)/imgui/src/*.cpp)
HEADERS = $(wildcard $(SRC)/**/*.h) $(wildcard $(SRC)/*/*.h) lib/pugixml-1.0/src/pugixml.hpp

default: $(TARGET) copyassets #run
all: default

$(BIN)/%.o: $(SRC)/%.cc
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		#$(CC) $(SRC)/stdafx.h -o $(BIN)/stdafx.h.gch $(OBJECTS) -Wall $(LIBS) -o $(BIN)/$@
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN)/$@

copyassets: ./assets
		-cp -r assets/ $(BIN)/assets

clean:
		-rm -rf $(BIN)/*


#.PHONY: run
#run: $(TARGET)
#		$(BIN)/$(TARGET)
