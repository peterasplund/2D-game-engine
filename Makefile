TARGET = peterspelmotor
#CC = clang++
CC = g++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image -L./lib/pugixml/src -L./lib/imgui/include
OPT=-O0
INCDIRS=-I/usr/include/SDL2 -I./lib/pugixml/src/ -I./lib/imgui/include/
CFLAGS = -Wwrite-strings -Wall -std=c++17 -MD -I/usr/local/include $(INCDIRS) $(OPT) -g
SRC=./src
LIB=./lib
BIN=./bin

.PHONY: default all clean

CFILES = $(shell find $(SRC) -type f -name '*.cc')
CFILES_LIBS = $(wildcard $(LIB)/imgui/src/*.cpp)
CFILES_LIBS += $(wildcard $(LIB)/pugixml/src/*.cpp)

OBJECTS = $(subst ./src/, ./bin/, $(patsubst %.cc, %.o, $(CFILES)))
OBJECTS += $(subst ./, ./bin/, $(patsubst %.cpp, %.o, $(CFILES_LIBS)))

default: $(TARGET) copyassets #run
all: default

# Compiling external deps
$(BIN)/lib/pugixml/src/%.o: $(LIB)/pugixml/src/%.cpp
		@mkdir -p $(@D)
		$(CC) -c $< -o $@
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
		-rm -rf $(BIN)/*

#.PHONY: run
#run: $(TARGET)
#		$(BIN)/$(TARGET)

-include $(OBJECTS:.o=.d)
