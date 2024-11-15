#pragma once

#include "map.h"
#include <string>
#include <nlohmann/json.hpp>

World createWorld(std::string filePath);
