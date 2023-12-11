#include "food.h"
#include "audio.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/event.h>
#include <stdlib.h>

DeclareComponentType(Food);

static void player_eat_food_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Position player_pos;
  {
    // only exist 1 player
    QueryIter iter = QueryWith(With(Player), Position);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    player_pos = *(Position *)comp[0].self;
    CComponent.query_free(&iter);
  }

  QueryIter iter = QueryWith(With(Food), Position);
  PComponent comp[1];
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {
    Position pos = *(Position *)comp[0].self;
    if (pos.x != player_pos.x || pos.y != player_pos.y)
      continue;

    play_sound(RTy(EatWav));
    PlayerEvent *event = malloc(sizeof(PlayerEvent));
    *event = PlayerEvent_Eat;
    event_emit(PlayerEvent, CEvent.default_vtable(event));
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void food_spawn(Position pos) {
  GameInfo *info = resource_get(GameInfo);
  Sprite sprite = {
      .eva_img = RTy(CherryEva),
      .active = true,
  };
  ScreenCord cord = {
      .x = pos.x * PLAYER_SIZE + info->offset_x,
      .y = pos.y * PLAYER_SIZE + info->offset_y,
      .z = 1,
  };
  Spawn(Food, Position, Sprite, ScreenCord, ComponentMarker, position_new(pos),
        sprite_new(sprite), screen_cord_new(cord));
}

void food_despawn() {
  QueryIter iter = QueryEntity(Food);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void food_init() {
  add_component_type(Food);
  AddUpdateSystem(player_eat_food_system);
}