#include "logger.h"
#include <stdarg.h>
#include <memory.h>

void log(LogLevel level, const char* message, ...) {
  char out_message[32000];
  memset(out_message, 0, sizeof(out_message));

  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  vsnprintf(out_message, 32000, message, arg_ptr);
  va_end(arg_ptr);

  sprintf(out_message, "%s%s %s\033[0;37m\n", levelColorCodes[level], levelStrings[level], message);
};
