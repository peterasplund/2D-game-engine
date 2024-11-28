#pragma once

#include "map.h"
#include <nlohmann/json.hpp>
#include <string>

World createWorld(std::string filePath);
