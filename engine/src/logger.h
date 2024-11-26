#pragma once

#include <string>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

enum LogLevel {
  LOG_LEVEL_FATAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARN = 2,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 4,
  LOG_LEVEL_TRACE = 5,
};

const char* levelStrings[6] = {
  "[FATAL]",
  "[ERROR]",
  "[WARN]",
  "[INFO]",
  "[DEBUG]",
  "[TRACE]",
};

const char* levelColorCodes[6] = {
  "\033[0;31m", // Red
  "\033[0;35m", // Purple
  "\033[0;33m", // yellow
  "\033[0;37m", // white
  "\033[0;36m", // Cyan
  "\033[0;34m", // blue
};

void log(LogLevel level, const char* message, ...);

#define LOG_INFO(message, ...) log(LogLevel::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#define LOG_ERROR(message, ...) log(LogLevel::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#define LOG_FATAL(message, ...) log(LogLevel::LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LOG_WARN(message, ...) log(LogLevel::LOG_LEVEL_WARN, message, ##__VA_ARGS__);
