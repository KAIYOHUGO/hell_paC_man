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

#define InternalAddSystem1(F, S1) F(S1);
#define InternalAddSystem2(F, S1, S2)                                          \
  InternalAddSystem1(F, S1) InternalAddSystem1(F, S2)

#define InternalAddSystem3(F, S1, S2, S3)                                      \
  InternalAddSystem2(F, S1, S2) InternalAddSystem1(F, S3)

#define InternalAddSystem4(F, S1, S2, S3, S4)                                  \
  InternalAddSystem3(F, S1, S2, S3) InternalAddSystem1(F, S4)

#define InternalAddSystem5(F, S1, S2, S3, S4, S5)                              \
  InternalAddSystem4(F, S1, S2, S3, S4) InternalAddSystem1(F, S5)

#define AddUpdateSystem(...)                                                   \
  {                                                                            \
    InternalConcatIdent(InternalAddSystem, InternalOverload(__VA_ARGS__))(     \
        CApp.add_update_system, __VA_ARGS__)                                   \
  }

#define AddRenderSystem(...)                                                   \
  {                                                                            \
    InternalConcatIdent(InternalAddSystem, InternalOverload(__VA_ARGS__))(     \
        CApp.add_render_system, __VA_ARGS__)                                   \
  }

#endif // __APP_H
