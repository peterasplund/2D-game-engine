# ---- config ----
CXX         := g++
OPT         ?= -O0
SRC         := ./src
LIB         := ./lib
BIN         := ./bin
TARGET      := $(BIN)/engine.so

# Flags
CPPFLAGS    := -I$(LIB)/imgui/include `sdl2-config --cflags`
CXXFLAGS    := -std=c++17 -Wall -Wwrite-strings -g $(OPT) -fPIC -MMD -MP
LDFLAGS     := -shared
LDLIBS      := -lm `sdl2-config --libs` -lSDL2_image

# ---- files ----
ENGINE_SRC  := $(shell find $(SRC) -type f -name '*.cc')
IMGUI_SRC   := $(wildcard $(LIB)/imgui/src/*.cpp)

ENGINE_OBJS := $(ENGINE_SRC:$(SRC)/%.cc=$(BIN)/%.o)
IMGUI_OBJS  := $(IMGUI_SRC:./%=$(BIN)/%)
IMGUI_OBJS  := $(IMGUI_OBJS:.cpp=.o)
OBJECTS     := $(ENGINE_OBJS) $(IMGUI_OBJS)

# ---- phony ----
.PHONY: default all clean

default: $(TARGET)
all: default

# ---- compile rules ----
# Engine .cc -> bin
$(BIN)/%.o: $(SRC)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# ImGui .cpp -> bin/lib/imgui/src
$(BIN)/lib/imgui/src/%.o: $(LIB)/imgui/src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# ---- link shared lib ----
$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

# ---- housekeeping ----
clean:
	-rm -rf $(BIN)

-include $(OBJECTS:.o=.d)
