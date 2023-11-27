#if !defined(__APP_C)
#define __APP_C

#include "app.h"
#include "basic.h"
#include "component.h"
#include "event.h"
#include <time.h>

struct App App = {};

static void internal_app_init() {
  f64 standard_delta = 1000.0 / 1.0;
  struct App app = {
      .on_update = vec_init(SystemFn),
      .on_render = vec_init(SystemFn),
      .standard_delta = standard_delta,
      .time_delta = standard_delta,
      .is_exit = false,
  };
  App = app;
}

static void internal_init_all() {
  internal_component_storage_init();
  internal_event_queue_init();
  internal_app_init();
}

static void internal_app_epoch() {
  struct timespec begin, end;
  clock_gettime(CLOCK_REALTIME, &begin);

  // update
  hook_app_before_update();
  CEvent.flush();
  for (usize i = 0; i < App.on_update.len; i++) {
    SystemFn fn = *vec_index(SystemFn, &App.on_update, i);
    fn();
  }
  hook_app_after_update();

  // render
  hook_app_before_render();
  for (usize i = 0; i < App.on_render.len; i++) {
    SystemFn fn = *vec_index(SystemFn, &App.on_render, i);
    fn();
  }
  hook_app_after_render();

  hook_app_render();

  clock_gettime(CLOCK_REALTIME, &end);
  f64 ms_delta =
      (end.tv_sec - begin.tv_sec) * 1e+3 + (end.tv_nsec - begin.tv_nsec) * 1e-6;
  if (ms_delta >= App.time_delta)
    return;
  f64 ms_sleep = App.time_delta - ms_delta;
  __time_t sec_part = ms_sleep / 1e3;
  __time_t nano_part = (ms_sleep - sec_part * 1e3) * 1e6;
  struct timespec sleep = {.tv_sec = sec_part, .tv_nsec = nano_part};
  nanosleep(&sleep, NULL);
}

void app_start() {
  internal_init_all();
  hook_app_init();

  while (!App.is_exit) {
    internal_app_epoch();
  }
  hook_app_free();
}

void raw_add_update_system(SystemFn fn) {
  vec_push(SystemFn, &App.on_update, fn);
}

void raw_add_render_system(SystemFn fn) {
  vec_push(SystemFn, &App.on_render, fn);
}

const struct CApp CApp = {
    .add_update_system = raw_add_update_system,
    .add_render_system = raw_add_render_system,
};

#endif // __APP_C