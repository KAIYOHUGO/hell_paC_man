#include "in_game.h"
#include "audio.h"
#include "booster.h"
#include "component.h"
#include "food.h"
#include "ghost.h"
#include "global.h"
#include "input.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <miniaudio.h>
#include <stdlib.h>

DeclareComponentType(RemainDisplay);

DeclareComponentType(LifeDisplay);

static void generate_map_system() {
  if (!state_is_in(GameState, GameState_GenerateMap))
    return;

  GameInfo *info = resource_get(GameInfo);

  Position player_pos = {
      .x = rand() % info->width,
      .y = rand() % info->height,
  };
  player_spawn(player_pos);

  usize area = info->height * info->width;
  usize remain_ghost = (area * GHost_Percentage + 99) / 100,
        remain_food = (area * Food_Percentage + 99) / 100,
        remain_booster = (area * Booster_Percentage + 99) / 100;
  info->food_amount = remain_food;

  while (remain_ghost > 0 || remain_food > 0 || remain_booster > 0) {
    for (usize x = 0; x < info->width; x++) {
      for (usize y = 0; y < info->height; y++) {
        if (player_pos.x == x && player_pos.y == y)
          continue;

        Position pos = {.x = x, .y = y};
        if (remain_food > 0 && rand() % 100 < Food_Percentage) {
          remain_food--;
          food_spawn(pos);
          continue;
        }
        if (remain_ghost > 0 && rand() % 100 < GHost_Percentage) {
          remain_ghost--;
          ghost_spawn(pos);
          continue;
        }
        if (remain_booster > 0 && rand() % 100 < Booster_Percentage) {
          remain_booster--;
          booster_spawn(pos);
          continue;
        }
      }
    }
  }
  state_set(GameState, GameState_InGame);
}

static void player_respawn_system() {
  if (!state_is_in(GameState, GameState_RespawnPlayer))
    return;
  GameInfo *info = resource_get(GameInfo);

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

static void spawn_display_system() {
  if (!state_is_exit(GameState, GameState_GenerateMap) &&
      !state_is_exit(GameState, GameState_Setting_ReadMap))
    return;
  GameInfo *info = resource_get(GameInfo);
  {
    Number num = {
        .len = 4,
        .n = info->food_amount,
    };
    ScreenCord cord = {
        .x = SCREEN_WIDTH / 2,
        .y = 2,
        .z = 10,
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
        .z = 10,
    };
    Spawn(LifeDisplay, Number, ScreenCord, ComponentMarker, number_new(num),
          screen_cord_new(cord));
  }
  play_sound(RTy(BeginningWav));
}

static void enter_in_game_system() {
  if (!state_is_enter(GameState, GameState_InGame))
    return;

  // Enable animation
  QueryIter iter = Query(AnimationCord);
  PComponent comp[1];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    AnimationCord *animation_cord = (AnimationCord *)comp[0].self;
    animation_cord->active = true;
  }
  CComponent.query_free(&iter);
}

static void eat_system() {
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

static void attacked_system() {
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

    // stop animation
    {
      QueryIter iter = Query(AnimationCord);
      PComponent comp[1];
      while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
        AnimationCord *animation_cord = (AnimationCord *)comp[0].self;
        animation_cord->active = false;
      }
      CComponent.query_free(&iter);
    }
    QueryIter iter = QueryWith(With(Ghost), Sprite);
    PComponent comp[1];
    while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
      Sprite *sprite = (Sprite *)comp[0].self;
      sprite->active = false;
    }
    CComponent.query_free(&iter);

    state_set(GameState, GameState_PlayerDie);
    break;
  }
}

static void display_update_system() {
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

static void back_to_menu_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Vec(PEvent) *events = event_listen(Key);
  for (usize i = 0; i < events->len; i++) {
    Key key = *(Key *)vec_index(PEvent, events, i)->self;
    if (key.kind != Key_ESC)
      continue;

    state_set(GameState, GameState_Menu);
  }
}

void in_game_display_despawn() {
  {
    QueryIter iter = QueryEntity(RemainDisplay);
    Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
    CComponent.despawn(id);
    CComponent.query_free(&iter);
  }
  QueryIter iter = QueryEntity(LifeDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.despawn(id);
  CComponent.query_free(&iter);
}

void in_game_init() {
  add_component_type(RemainDisplay);
  add_component_type(LifeDisplay);
  AddUpdateSystem(generate_map_system, player_respawn_system,
                  spawn_display_system, enter_in_game_system);
  AddUpdateSystem(eat_system, attacked_system, display_update_system);
  AddUpdateSystem(back_to_menu_system);
}
