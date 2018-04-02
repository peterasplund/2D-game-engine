TARGET = bin/main
CC = clang++
LIBS = -lm `sdl2-config --libs --cflags` -lSDL2_image
CFLAGS = -I/usr/local/include

.PHONY: default all clean

default: $(TARGET)
# default: clean $(TARGET)
all: default

OBJECTS = $(patsubst %.cc, %.o, $(wildcard *.cc))
HEADERS = $(wildcard *.h)

%.o: %.cc $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
		$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
		-rm -f *.o
		-rm -f $(TARGET)
