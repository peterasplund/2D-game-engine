TARGET = main
CC = clang++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS = -Wwritable-strings -std=c++11 -I/usr/local/include
SRC=./src
BIN=./bin

.PHONY: default all clean

OBJECTS = $(patsubst $(SRC)/%.cc, $(BIN)/%.o, $(wildcard $(SRC)/*.cc))
HEADERS = $(wildcard $(SRC)*.h)

default: clean $(TARGET) #run
all: default

$(BIN)/%.o: $(SRC)/%.cc $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN)/$@

clean:
		-rm -f $(BIN)/*


#.PHONY: run
#run: $(TARGET)
#		$(BIN)/$(TARGET)
