#if !defined(SETTING_C)
#define SETTING_C

#include "setting.h"
#include "component.h"
#include "global.h"
#include "input.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdlib.h>

DeclareResourceType(GameInfo);
DeclareComponentType(MapCursor);

void read_height() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Setting_ReadHeight)
    return;
  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER) {
      *game_state = GameState_Setting_ReadWidth;
      return;
    }

    if (key.kind != Key_Number)
      continue;
    info->height *= 10;
    info->height += key.number;
  }
}

void read_width() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Setting_ReadWidth)
    return;
  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER) {
      *game_state = GameState_Enter_Setting_ReadMap;
      return;
    }

    if (key.kind != Key_Number)
      continue;
    info->width *= 10;
    info->width += key.number;
  }
}

void enter_read_map() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Enter_Setting_ReadMap)
    return;
  GameInfo *info = resource_get(GameInfo);
  info->offset_x = 20;
  info->offset_y = 20;

  // spawn background
  Sprite block_sprite = {.eva_img = RTy(BlockEva), .active = true};
  ScreenCord block_cord = {
      .z = 0,
  };
  for (usize y = 0; y < info->height; y++) {
    for (usize x = 0; x < info->width; x++) {
      Position pos = {
          .x = x,
          .y = y,
      };
      Spawn(Sprite, Position, ScreenCord, sprite_new(block_sprite),
            position_new(pos), screen_cord_new(block_cord));
    }
  }

  Position pos = {
      .x = 0,
      .y = 0,
  };
  Sprite sprite = {.eva_img = RTy(CursorEva), .active = true};
  ScreenCord cord = {
      .z = 2,
  };

  Spawn(MapCursor, Position, Sprite, ScreenCord, ComponentMarker,
        position_new(pos), sprite_new(sprite), screen_cord_new(cord));
  player_spawn(pos);

  *game_state = GameState_Setting_ReadMap;
}

void read_map() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Setting_ReadMap)
    return;
  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  // only exist 1 cursor
  QueryIter iter = QueryWith(With(MapCursor), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER) {
      *game_state = GameState_Exit_Setting_ReadMap;
      return;
    }

    if (key.kind != Key_Char)
      continue;
    switch (key.charater) {
    case 'w':
      if (pos->y == 0)
        break;
      pos->y--;
      break;
    case 's':
      if (pos->y + 1 == info->height)
        break;
      pos->y++;
      break;
    case 'a':
      if (pos->x == 0)
        break;
      pos->x--;
      break;
    case 'd':
      if (pos->x + 1 == info->width)
        break;
      pos->x++;
      break;
    case 'p':
      player_move(*pos);
      break;
    case 'b':
      break;
    case 'm':
      break;
    case 'f':
      break;
    case ' ':
      break;
    }
  }
}

void exit_input_map() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Exit_Setting_ReadMap)
    return;

  // only exist 1 cursor
  QueryIter iter = QueryEntity(MapCursor);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.query_free(&iter);
  CComponent.despawn(id);
  *game_state = GameState_InGame;
}

void setting_init() {
  add_resource_type(GameInfo);
  add_component_type(MapCursor);

  GameInfo *info = malloc(sizeof(GameInfo));
  info->height = 0;
  info->width = 0;
  resource_insert(GameInfo, info);
  AddUpdateSystem(exit_input_map, read_map, enter_read_map, read_width,
                  read_height);
}

#endif // SETTING_C