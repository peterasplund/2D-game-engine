#include "event.h"
#include "logger.h"
#include <vector>

typedef struct registered_event {
  void* listener;
  PFN_on_event callback;
} registered_event;

typedef struct event_code_entry {
  std::vector<registered_event> events;
} event_code_entry;

#define MAX_MESSAGE_CODES 16384

typedef struct event_system_state {
  // Loookup table for event codes.
  event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

static bool is_initialized = false;
static event_system_state state;

bool event_initialize() {
  if (is_initialized == true) {
    return false;
  }

  is_initialized = true;

  state = event_system_state();

  return true;
}

void event_shutdown() {
  for (u16 i = 0; i < MAX_MESSAGE_CODES; ++i) {
    if (state.registered[i].events.size() != 0) {
      state.registered[i].events.clear();
    }
  }
}

bool event_register(u16 code, void* listener, PFN_on_event on_event) {
  if (is_initialized == false) {
		LOG_ERROR("Event system is uninitialized");
    return false;
  }

  u64 registered_count = state.registered[code].events.size();
  for (u64 i = 0; i < registered_count; ++i) {
    if (state.registered[code].events[i].listener == listener) {
      LOG_WARN("Event with code \"%d\" already registered for this listener.", code);
      return false;
    }
  }

  registered_event event;
  event.listener = listener;
  event.callback = on_event;

  state.registered[code].events.push_back(event);

  return true;
}

bool event_unregister(u16 code, void* listener, PFN_on_event on_event) {
  if (is_initialized == false) {
    return false;
  }

  if (state.registered[code].events.size() == 0) {
    LOG_WARN("Event with code \"%d\" for this listener isn't registered. Cannot unregister.", code);
    return false;
  }

  u64 registered_count = state.registered[code].events.size();
  for (u64 i = 0; i < registered_count; ++i) {
    registered_event e = state.registered[code].events[i];
    if (e.listener == listener && e.callback == on_event) {
      // Found one, remove it
      state.registered[code].events.erase(
        state.registered[code].events.begin() + i
      );

      return true;
    }
  }

  // Not found.
  return false;
}

bool event_fire(u16 code, void* sender, event_context context) {
  if (is_initialized == false) {
		LOG_ERROR("Event system is uninitialized");
    return false;
  }

  if (state.registered[code].events.size() == 0) {
    return false;
  }

  u64 registered_count = state.registered[code].events.size();
  for (u64 i = 0; i < registered_count; ++i) {
    registered_event e = state.registered[code].events[i];
    if (e.callback(code, sender, e.listener, context)) {
      // Message has been handled, do not send to other listeners.
      return true;
    }
  }

  // Not found.
  return false;
}
