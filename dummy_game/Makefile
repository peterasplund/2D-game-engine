# ---- config ----
CXX         := g++
OPT         ?= -O0
SRC         := ./src
BIN         := ./bin

# Where your engine lives
ENGINE_DIR  ?= ../engine
ENGINE_SO   := $(ENGINE_DIR)/bin/engine.so   # matches your engine Makefile output

TARGET      := $(BIN)/dummy_game

# ---- flags ----
# Add your engine's public headers if you have them, e.g. $(ENGINE_DIR)/include
CPPFLAGS    := `sdl2-config --cflags` -I$(ENGINE_DIR)/src -I$(ENGINE_DIR)/lib/imgui/include
CXXFLAGS    := -std=c++17 -Wall -Wwrite-strings -g $(OPT) -MMD -MP
LDFLAGS     := -Wl,-rpath,'$$ORIGIN'        # so the binary finds engine.so in the same dir
LDLIBS      := `sdl2-config --libs` -lSDL2_image -lm

# ---- sources/objects ----
GAME_SRC    := $(shell find $(SRC) -type f -name '*.cc')
OBJECTS     := $(GAME_SRC:$(SRC)/%.cc=$(BIN)/%.o)

# ---- phony ----
.PHONY: default all clean run copyassets copyengine

default: $(TARGET) copyassets
all: default

# ---- compile ----
$(BIN)/%.o: $(SRC)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# ---- link ----
# Link directly with the full path to engine.so
$(TARGET): $(OBJECTS) $(ENGINE_SO)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(ENGINE_SO) $(LDLIBS) -o $@

# ---- copy runtime deps ----
# Put engine.so next to the game binary so rpath '$ORIGIN' can find it.
copyengine: $(ENGINE_SO)
	@mkdir -p $(BIN)
	cp $(ENGINE_SO) $(BIN)/

copyassets: ./assets
	-cp -r assets $(BIN)/assets

# Convenience: ensure engine.so is copied whenever we build the game
$(TARGET): copyengine

clean:
	-rm -rf $(BIN)

-include $(OBJECTS:.o=.d)
