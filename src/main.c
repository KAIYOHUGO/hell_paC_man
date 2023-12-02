#if !defined(MAIN_C)
#define MAIN_C

#include "component.h"
#include "debug.h"
#include "ghost.h"
#include "input.h"
#include "player.h"
#include "render.h"
#include "setting.h"
#include "state.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/resource.h>
#include <stdio.h>

void hook_app_init() {
  printf("Init!\n");

  add_component_type(Ghost);

  add_event_type(Key);

  RGB clean_color = {
      .r = 255,
      .g = 0,
      .b = 0,
  };
  // render_init(30, 92, clean_color);
  render_init(47, 211, clean_color);
  state_init();
  component_init();
  player_init();
  setting_init();
};

void hook_app_before_update() { listen_keyboard(); };
void hook_app_after_update(){
    // sprite_debug();
    // setting_debug();
    // state_debug();
};
void hook_app_before_render(){};
void hook_app_after_render(){};

void hook_app_render() { render(); };
void hook_app_free(){

};

isize main() {
  struct termios old_term = init_terminal();
  app_start();
  reset_terminal(old_term);
}
#endif // MAIN_C
