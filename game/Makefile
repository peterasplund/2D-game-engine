TARGET = game
CC = g++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image -L./lib/imgui/include -L../bin -lengine -Wl,-rpath,.
OPT=-O0
INCDIRS=-I/usr/include/SDL2 -I./lib/imgui/include/ -I../engine/src
CFLAGS = -Wwrite-strings -Wall -std=c++17 -MD -I/usr/local/include $(INCDIRS) $(OPT) -g -fPIC
SRC=./src
LIB=./lib
BIN=../bin

.PHONY: default all clean

CFILES = $(shell find $(SRC) -type f -name '*.cc')
CFILES_LIBS = $(wildcard $(LIB)/imgui/src/*.cpp)

OBJECTS = $(subst ./src/, ../bin/, $(patsubst %.cc, %.o, $(CFILES)))
OBJECTS += $(subst ./, ../bin/, $(patsubst %.cpp, %.o, $(CFILES_LIBS)))

# Compiling external deps
$(BIN)/lib/imgui/src/%.o: $(LIB)/imgui/src/%.cpp
		@mkdir -p $(@D)
		$(CC) -I/usr/include/SDL2 -c $< -o $@

# Compiling game
$(BIN)/%.o: $(SRC)/%.cc
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/components/%.o: $(SRC)/components/%.cc
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/scene/%.o: $(SRC)/scene/%.cc
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN)/$@

copyassets: ./assets
		-cp -r assets $(BIN)/assets

clean:
		-rm -rf $(BIN)/engine.so

-include $(OBJECTS:.o=.d)
