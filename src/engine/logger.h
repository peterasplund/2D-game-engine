#pragma once

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

enum LogLevel {
  FATAL = 0,
  ERROR = 1,
  WARN = 2,
  INFO = 3,
  DEBUG = 4,
  TRACE = 5,
};

void log(LogLevel level, const char *message, ...);

#define LOG_TRACE(message, ...) log(LogLevel::TRACE, message, ##__VA_ARGS__);
#define LOG_INFO(message, ...) log(LogLevel::INFO, message, ##__VA_ARGS__);
#define LOG_DEBUG(message, ...) log(LogLevel::DEBUG, message, ##__VA_ARGS__);
#define LOG_ERROR(message, ...) log(LogLevel::ERROR, message, ##__VA_ARGS__);
#define LOG_FATAL(message, ...) log(LogLevel::FATAL, message, ##__VA_ARGS__);
#define LOG_WARN(message, ...) log(LogLevel::WARN, message, ##__VA_ARGS__);
