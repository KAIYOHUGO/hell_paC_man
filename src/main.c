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
  add_component_type(Position);

  add_component_type(Player);
  add_component_type(Ghost);

  add_event_type(Key);

  add_resource_type(GameInfo);
  add_resource_type(GameState);

  GameState *state = malloc(sizeof(GameState));
  *state = GameState_InGame;
  // *state = GameState_Menu;
  resource_insert(GameState, state);

  player_init();
};

void hook_app_before_update() { listen_keyboard(); };
void hook_app_after_update() { sprite_debug(); };
void hook_app_before_render(){};
void hook_app_after_render(){};

void hook_app_render(){

};
void hook_app_free(){};

isize main() {
  struct termios old_term = init_terminal();
  app_start();
  reset_terminal(old_term);
}
#endif // MAIN_C
