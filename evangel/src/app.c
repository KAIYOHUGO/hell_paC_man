#if !defined(__APP_C)
#define __APP_C

#include "app.h"
#include "basic.h"

void internal_app_init() {
  internal_component_storage_init();
  // todo
}

struct App App = {};

#endif // __APP_C