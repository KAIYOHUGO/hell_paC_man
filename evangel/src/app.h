#if !defined(__APP_H)
#define __APP_H

#include "basic.h"
#include "vec.h"

extern void hook_app_init();

extern void hook_app_before_update();
extern void hook_app_after_update();
extern void hook_app_before_render();
extern void hook_app_after_render();

extern void hook_app_render();
extern void hook_app_free();

typedef void (*SystemFn)();

struct App {
  Vec(SystemFn) on_update;
  Vec(SystemFn) on_render;
  f64 standard_delta;
  f64 time_delta;
  bool is_exit;
};

struct CApp {
  void (*add_update_system)(SystemFn fn);
  void (*add_render_system)(SystemFn fn);
};

void app_start();

extern const struct CApp CApp;
#endif // __APP_H
