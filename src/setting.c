#include "setting.h"
#include "component.h"
#include "ghost.h"
#include "global.h"
#include "input.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdlib.h>

DeclareResourceType(GameInfo);
DeclareComponentType(MapCursor);
DeclareComponentType(HeightNum);
DeclareComponentType(WidthNum);

void enter_read_height() {
  GameState *game_state = resource_get(GameState);
  if (*game_state != GameState_Enter_Setting_ReadHeight)
    return;
  isize center = SCREEN_WIDTH / 2;

  Number height = {
      .n = 0,
      .len = 2,
  };
  ScreenCord height_cord = {.x = center - NUMBER_SIZE * 3, .y = 4};
  Spawn(HeightNum, Number, ScreenCord, ComponentMarker, number_new(height),
        screen_cord_new(height_cord));

  Number width = {
      .n = 0,
      .len = 2,
  };
  ScreenCord width_cord = {.x = center + NUMBER_SIZE, .y = 4};
  Spawn(WidthNum, Number, ScreenCord, ComponentMarker, number_new(width),
        screen_cord_new(width_cord));

  *game_state = GameState_Setting_ReadHeight;
}

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

    if (key.kind != Key_Number || info->height > 9)
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

    if (key.kind != Key_Number || info->width > 9)
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
  info->offset_x = (SCREEN_WIDTH - PLAYER_SIZE * (isize)info->width) / 2;
  info->offset_y = (SCREEN_HIGHT - PLAYER_SIZE * (isize)info->height) / 2;

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
      ghost_spawn(*pos);
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

  {
    // only exist 1 cursor
    QueryIter iter = QueryEntity(MapCursor);
    Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
    CComponent.query_free(&iter);
    CComponent.despawn(id);
    *game_state = GameState_InGame;
  }
  // despawn number display
  QueryIter iter = QueryEntity(Number);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void update_height_system() {
  GameState game_state = *resource_get(GameState);
  if (game_state != GameState_Setting_ReadHeight &&
      game_state != GameState_Setting_ReadWidth)
    return;

  GameInfo *info = resource_get(GameInfo);

  {
    QueryIter iter = QueryWith(With(HeightNum), Number);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    ((Number *)comp[0].self)->n = info->height;
    CComponent.query_free(&iter);
  }
  QueryIter iter = QueryWith(With(WidthNum), Number);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  ((Number *)comp[0].self)->n = info->width;
  CComponent.query_free(&iter);
}

void setting_init() {
  add_resource_type(GameInfo);
  add_component_type(MapCursor);
  add_component_type(HeightNum);
  add_component_type(WidthNum);

  GameInfo *info = malloc(sizeof(GameInfo));
  info->height = 0;
  info->width = 0;
  resource_insert(GameInfo, info);
  AddUpdateSystem(exit_input_map, read_map, enter_read_map);
  AddUpdateSystem(update_height_system, read_width, read_height,
                  enter_read_height);
}
