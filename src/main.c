#if !defined(MAIN_C)
#define MAIN_C

#include "component.h"
#include "debug.h"
#include "ghost.h"
#include "input.h"
#include "player.h"
#include "render.h"
#include "setting.h"
#include "global.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/resource.h>
#include <stdio.h>

void hook_app_init() {
  // printf("Init!\n");
  CApp.set_fps(60);
  add_component_type(Ghost);

  add_event_type(Key);

  RGB clean_color = {
      .r = 9,
      .g = 9,
      .b = 9,
  };
  // render_init(50, 100, clean_color);
  render_init(67, 240, clean_color);
  // render_init(30, 120, clean_color);
  // render_init(50, 192, clean_color);
  // render_init(56, 213, clean_color);
  // render_init(48, 211, clean_color);
  global_init();
  component_init();
  player_init();
  setting_init();

  // ResourceType ty = CResource.add_new_type();
  // CResource.insert(ty, open_eva("assets/out.eva"));
  // Sprite sprite = {.eva_img = ty};
  // ScreenCord cord = {
  //     .x = -9,
  //     .y = 70,
  //     .z = 0,
  // };
  // Spawn(Sprite, ScreenCord, sprite_new(sprite), screen_cord_new(cord));
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
#endif // MAIN_C
