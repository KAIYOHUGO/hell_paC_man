#if !defined(__APP_H)
#define __APP_H

#include "basic.h"
#include "component.h"
#include "event.h"
#include "map.h"
#include "vec.h"

extern void hook_app_init();
extern void hook_app_on_update();
extern void hook_app_on_render();
extern void hook_app_free();

typedef void (*system_fn)();

struct App {
  struct EventQueue event_queue;
  Vec(system_fn) on_update;
};

struct CApp {};

extern struct App App;

extern const struct CApp CApp;

void internal_app_init();

#endif // __APP_H
