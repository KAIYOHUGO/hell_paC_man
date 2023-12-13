#include "audio.h"
#include "booster.h"
#include "component.h"
#include "food.h"
#include "ghost.h"
#include "global.h"
#include "in_game.h"
#include "input.h"
#include "lose.h"
#include "menu.h"
#include "player.h"
#include "render.h"
#include "setting.h"
#include "win.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/resource.h>
#include <stdlib.h>

void hook_app_init() {
  CApp.set_fps(60);

  add_event_type(Key);

  RGB clean_color = {
      .r = 0x21,
      .g = 0x21,
      .b = 0xde,
  };
  render_init(SCREEN_LINE, SCREEN_WIDTH, clean_color);
  audio_init();

  global_init();

  component_init();
  player_init();
  ghost_init();
  food_init();
  booster_init();

  setting_init();
  in_game_init();
  menu_init();
  lose_init();
  win_init();
};

void hook_app_before_update() { listen_keyboard(); };

void hook_app_after_update(){};
void hook_app_before_render(){};
void hook_app_after_render(){};

void hook_app_render() {
  // render
  render();
};

void hook_app_free(){};

isize main() {
  srand(time(NULL));
  struct termios old_term = init_terminal();
  app_start();
  reset_terminal(old_term);
}
