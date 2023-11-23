#if !defined(__MAIN_C)
#define __MAIN_C

#include "app.h"
#include "basic.h"

isize main() {
  hook_app_init();
  hook_app_on_render();
  hook_app_on_update();
  hook_app_free();
  return 0;
}

#endif // __MAIN_C
