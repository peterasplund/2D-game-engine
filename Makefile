TARGET = main
CC = clang++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS = -Wwritable-strings -std=c++17 -I/usr/local/include -I/opt/local/include/SDL2
SRC=./src
BIN=./bin

.PHONY: default all clean

OBJECTS = $(patsubst $(SRC)/%.cc, $(BIN)/%.o, $(wildcard $(SRC)/*.cc))
HEADERS = $(wildcard $(SRC)*.h)

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


#.PHONY: run
#run: $(TARGET)
#		$(BIN)/$(TARGET)
