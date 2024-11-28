#pragma once

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

void log(LogLevel level, const char* message, ...);

#define LOG_TRACE(message, ...) log(LogLevel::LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#define LOG_INFO(message, ...) log(LogLevel::LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#define LOG_DEBUG(message, ...) log(LogLevel::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#define LOG_ERROR(message, ...) log(LogLevel::LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#define LOG_FATAL(message, ...) log(LogLevel::LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define LOG_WARN(message, ...) log(LogLevel::LOG_LEVEL_WARN, message, ##__VA_ARGS__);
