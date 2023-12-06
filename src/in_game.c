#include "in_game.h"
#include "component.h"
#include "ghost.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/resource.h>

void generate_map() {
  GameInfo *info = resource_get(GameInfo);

  Position player_pos = {
      .x = rand() % info->width,
      .y = rand() % info->height,
  };
  player_spawn(player_pos);

  usize area = info->height * info->width;
  usize max_ghost = (area * GHost_Percentage + 99) / 100,
        max_food = (area * Food_Percentage + 99) / 100,
        max_booster = (area * Booster_Percentage + 99) / 100;

  for (usize x = 0; x < info->width; x++) {
    for (usize y = 0; y < info->height; y++) {
      if (player_pos.x == x && player_pos.y == y)
        continue;

      usize r = rand() % 100;
      Position pos = {.x = x, .y = y};
      if (max_ghost > 0 && r < GHost_Percentage) {
        max_ghost--;
        ghost_spawn(pos);
      } else if (max_food > 0 && r < Food_Percentage) {
        max_food--;
        // spawn food
      } else if (max_booster > 0 && r < Booster_Percentage) {
        max_booster--;
        // spawn booster
      }
    }
  }
}

void enter_in_game() {
  if (!state_is_enter(GameState, GameState_InGame))
    return;

  GameInfo *info = resource_get(GameInfo);
  switch (info->mod) {
  case GameMode_Default:
    generate_map();
    break;
  case GameMode_Custom:
    break;
  }
  state_set(GameState, GameState_InGame);
}

void in_game_init() { AddUpdateSystem(enter_in_game); }
