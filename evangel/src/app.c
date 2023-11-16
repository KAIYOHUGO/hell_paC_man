#if !defined(__APP_C)
#define __APP_C

#include "app.h"
#include "basic.h"

isize main() {
  hook_app_init();
  hook_app_on_render();
  hook_app_on_update();
  hook_app_free();
  return 0;
}

#endif // __APP_C