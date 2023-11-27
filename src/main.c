#if !defined(MAIN_C)
#define MAIN_C

#include "component.h"
#include "ghost.h"
#include "input.h"
#include "player.h"
#include "render.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <stdio.h>

void hook_app_init() {
  printf("Init!\n");
  add_component_type(Position);

  add_component_type(Player);
  add_component_type(Ghost);

  player_init();
};

void hook_app_before_update() { listen_keyboard(); };
void hook_app_after_update(){};
void hook_app_before_render(){};
void hook_app_after_render(){};

void hook_app_render() { printf("R\n"); };
void hook_app_free(){};

isize main() {
  struct termios old_term = init_terminal();
  app_start();
  reset_terminal(old_term);
}
#endif // MAIN_C
