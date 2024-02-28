#pragma once

#include "pugixml.hpp"
#include "tilemap.h"

Tilemap* tiled_load_map(const char* mapFile);
