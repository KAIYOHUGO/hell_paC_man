#include "in_game.h"
#include "audio.h"
#include "booster.h"
#include "component.h"
#include "food.h"
#include "ghost.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/resource.h>
#include <miniaudio.h>
#include <stdlib.h>

DeclareComponentType(RemainDisplay);

DeclareComponentType(LifeDisplay);

void generate_map() {
  if (!state_is_in(GameState, GameState_GenerateMap))
    return;

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
  usize tmp = max_food;

  for (usize x = 0; x < info->width; x++) {
    for (usize y = 0; y < info->height; y++) {
      if (player_pos.x == x && player_pos.y == y)
        continue;

      Position pos = {.x = x, .y = y};
      if (max_food > 0 && rand() % 100 < Food_Percentage) {
        max_food--;
        food_spawn(pos);
        continue;
      }
      if (max_ghost > 0 && rand() % 100 < GHost_Percentage) {
        max_ghost--;
        ghost_spawn(pos);
        continue;
      }
      if (max_booster > 0 && rand() % 100 < Booster_Percentage) {
        max_booster--;
        booster_spawn(pos);
        continue;
      }
    }
  }
  info->food_amount = tmp - max_food;
  state_set(GameState, GameState_InGame);
}

void player_respawn() {
  if (!state_is_in(GameState, GameState_RespawnPlayer))
    return;
  GameInfo *info = resource_get(GameInfo);

  player_despawn();
  Position player_pos = {
      .x = rand() % info->width,
      .y = rand() % info->height,
  };
  player_spawn(player_pos);

  usize ghost_amount = 0;
  QueryIter iter = QueryEntity(Ghost);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
    ghost_amount++;
  }
  CComponent.query_free(&iter);

  while (ghost_amount > 0) {
    for (usize x = 0; x < info->width; x++) {
      for (usize y = 0; y < info->height; y++) {
        if (player_pos.x == x && player_pos.y == y)
          continue;

        Position pos = {.x = x, .y = y};
        if (ghost_amount > 0 && rand() % 100 < GHost_Percentage) {
          ghost_amount--;
          ghost_spawn(pos);
          continue;
        }
      }
    }
  }

  state_set(GameState, GameState_InGame);
}

void spawn_display() {
  if (!state_is_exit(GameState, GameState_GenerateMap))
    return;

  GameInfo *info = resource_get(GameInfo);
  info->life = 2;

  {
    Number num = {
        .len = 4,
        .n = info->food_amount,
    };
    ScreenCord cord = {
        .x = SCREEN_WIDTH / 2,
        .y = 2,
    };
    Spawn(RemainDisplay, Number, ScreenCord, ComponentMarker, number_new(num),
          screen_cord_new(cord));
  }
  {
    Number num = {
        .len = 2,
        .n = info->life,
    };
    ScreenCord cord = {
        .x = SCREEN_WIDTH / 2 - 4 * NUMBER_SIZE,
        .y = 2,
    };
    Spawn(LifeDisplay, Number, ScreenCord, ComponentMarker, number_new(num),
          screen_cord_new(cord));
  }
}

void enter_in_game() {
  if (!state_is_enter(GameState, GameState_InGame))
    return;

  // Fix cord
  QueryIter iter = Query(AnimationCord, ScreenCord);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    AnimationCord *animation_cord = (AnimationCord *)comp[0].self;
    ScreenCord *cord = (ScreenCord *)comp[1].self;
    animation_cord->active = true;
    cord->x = animation_cord->x;
    cord->y = animation_cord->y;
  }
  CComponent.query_free(&iter);

  play_sound(RTy(BeginningWav));
}

void eat_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;
  Vec(PEvent) *p_events = event_listen(PlayerEvent);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PlayerEvent event = *(PlayerEvent *)vec_index(PEvent, p_events, i)->self;
    if (event != PlayerEvent_Eat)
      continue;

    info->food_amount--;
  }
  if (info->food_amount == 0)
    state_set(GameState, GameState_Win);
}

void attacked_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;
  Vec(PEvent) *p_events = event_listen(PlayerEvent);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PlayerEvent event = *(PlayerEvent *)vec_index(PEvent, p_events, i)->self;
    if (event != PlayerEvent_Die)
      continue;

    info->life--;
    if (info->life > 0) {
      state_set(GameState, GameState_RespawnPlayer);
    } else {
      state_set(GameState, GameState_Lose);
    }
    break;
  }
}

void display_update() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  GameInfo *info = resource_get(GameInfo);
  {
    // only exist 1 display
    QueryIter iter = QueryWith(With(RemainDisplay), Number);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    CComponent.query_free(&iter);
    ((Number *)comp[0].self)->n = info->food_amount;
  }
  {
    // only exist 1 display
    QueryIter iter = QueryWith(With(LifeDisplay), Number);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    CComponent.query_free(&iter);
    ((Number *)comp[0].self)->n = info->life;
  }
}

void in_game_init() {
  add_component_type(RemainDisplay);
  add_component_type(LifeDisplay);
  AddUpdateSystem(generate_map, player_respawn, spawn_display, enter_in_game);
  AddUpdateSystem(eat_system, attacked_system, display_update);
}
