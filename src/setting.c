#if !defined(SETTING_C)
#define SETTING_C

#include "setting.h"
#include "component.h"
#include "input.h"
#include "state.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdlib.h>

DeclareResourceType(GameInfo);
DeclareComponentType(MapCursor);

void input_height() {
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

void input_width() {
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

void enter_input_map() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Enter_Setting_ReadMap)
    return;
  GameInfo *info = resource_get(GameInfo);

  Array(MapItem) map = array_init(MapItem, info->height * info->width);
  for (usize i = 1; i < map.len; i++) {
    *array_index(MapItem, &map, i) = MapItem_Empty;
  }
  *array_index(MapItem, &map, 0) = MapItem_Player;
  info->map = map;

  Position pos = {
      .x = 0,
      .y = 0,
  };
  PComponent p_pos = position_new(pos);

  Spawn(MapCursor, Position, ComponentMarker, p_pos);

  *game_state = GameState_Setting_ReadMap;
}

void input_map() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Setting_ReadMap)
    return;
  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);
  MapItem *map = array_typed(MapItem, &info->map);

  // only exist 1 cursor
  QueryIter iter = QueryWith(With(MapCursor), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;
    MapItem *point_at = map + pos->y * info->width + pos->x;

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
      if (*point_at != MapItem_Empty)
        break;

      // find old player
      for (usize i = 0; i < info->map.len; i++) {
        if (map[i] != MapItem_Player)
          continue;
        map[i] = MapItem_Empty;
        break;
      }
      *point_at = MapItem_Player;
      break;
    case 'b':
      if (*point_at != MapItem_Empty)
        break;
      *point_at = MapItem_Booster;
      break;
    case 'm':
      if (*point_at != MapItem_Empty)
        break;
      *point_at = MapItem_Monster;
      break;
    case 'f':
      if (*point_at != MapItem_Empty)
        break;
      *point_at = MapItem_Food;
      break;
    case ' ':
      if (*point_at == MapItem_Empty || *point_at == MapItem_Player)
        break;
      *point_at = MapItem_Empty;
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
  info->map = array_empty(MapItem);
  resource_insert(GameInfo, info);
  AddUpdateSystem(exit_input_map, input_map, enter_input_map, input_width,
                  input_height);
}

#endif // SETTING_C