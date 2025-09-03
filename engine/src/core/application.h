#pragma once

#include "defines.h"
#include "window.h"
#include "renderer.h"

typedef struct app_config {
	i16 window_x;
	i16 window_y;

	i16 window_width;
	i16 window_height;

	i16 zoom;

	const char* window_name;

	bool vsync;

	u16 max_frame_rate;
} app_config;

class Application {
public:
	Application();
	~Application();
  static Application *Instance();
	bool initialize(app_config* config);
	void run();
	Window* window;
	Renderer* renderer;

	void beforeUpdate();
	void afterUpdate();

	f64 getDeltaTime() {
		return delta_time;
	}

protected:
	bool is_initialized;

	bool is_running;
	bool is_suspended;
	f64 last_time;

	app_config config;

  u64 now;
  u64 last;
  u64 end;

  f64 delta_time = 0;
};
