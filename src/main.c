#include "component.h"
#include "debug.h"
#include "ghost.h"
#include "global.h"
#include "input.h"
#include "player.h"
#include "render.h"
#include "setting.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/resource.h>
#include <stdio.h>

void hook_app_init() {
  // printf("Init!\n");
  CApp.set_fps(60);

  add_event_type(Key);

  RGB clean_color = {
      .r = 0x21,
      .g = 0x21,
      .b = 0xde,
  };
  render_init(SCREEN_LINE, SCREEN_WIDTH, clean_color);

  global_init();
  component_init();
  player_init();
  ghost_init();
  setting_init();
};

void hook_app_before_update() { listen_keyboard(); };

void hook_app_after_update(){
    // QueryIter iter = Query(ScreenCord);
    // PComponent comp[1];
    // CComponent.query_next(&iter, array_ref(comp));
    // ScreenCord *cord = (ScreenCord *)(comp[0].self);
    // cord->x = (cord->x + 9 + 1) % 249 - 9;

    // CComponent.query_free(&iter);
    // sprite_debug();
    // setting_debug();
    // state_debug();
};
void hook_app_before_render(){};
void hook_app_after_render(){};

void hook_app_render() {
  // render
  render();
};

void hook_app_free(){};

isize main() {
  struct termios old_term = init_terminal();
  app_start();
  reset_terminal(old_term);
}
