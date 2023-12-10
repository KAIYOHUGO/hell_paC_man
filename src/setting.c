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

DeclareComponentType(MapCursorDisplay);
DeclareComponentType(HeightDisplay);
DeclareComponentType(WidthDisplay);

void enter_read_height() {
  if (!state_is_enter(GameState, GameState_Setting_ReadHeight))
    return;

  isize center = SCREEN_WIDTH / 2;

  Number height = {
      .n = 0,
      .len = 2,
  };
  ScreenCord height_cord = {.x = center - NUMBER_SIZE * 3, .y = 4};
  Spawn(HeightDisplay, Number, ScreenCord, ComponentMarker, number_new(height),
        screen_cord_new(height_cord));

  Number width = {
      .n = 0,
      .len = 2,
  };
  ScreenCord width_cord = {.x = center + NUMBER_SIZE, .y = 4};
  Spawn(WidthDisplay, Number, ScreenCord, ComponentMarker, number_new(width),
        screen_cord_new(width_cord));
}

void read_height() {
  if (!state_is_in(GameState, GameState_Setting_ReadHeight))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER && info->height > 1) {
      state_set(GameState, GameState_Setting_ReadWidth);
      return;
    }

    if (key.kind != Key_Number || info->height > 9)
      continue;
    info->height *= 10;
    info->height += key.number;
  }
}

void read_width() {
  if (!state_is_in(GameState, GameState_Setting_ReadWidth))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER && info->width > 1) {
      switch (info->mod) {
      case GameMode_Custom:
        state_set(GameState, GameState_Setting_ReadMap);
        break;
      case GameMode_Default:
        state_set(GameState, GameState_GenerateMap);
        break;
      }
      return;
    }

    if (key.kind != Key_Number || info->width > 9)
      continue;
    info->width *= 10;
    info->width += key.number;
  }
}

void exit_read_width() {
  if (!state_is_exit(GameState, GameState_Setting_ReadWidth))
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

  // despawn number display
  QueryIter iter = QueryEntity(Number);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void enter_read_map() {
  if (!state_is_enter(GameState, GameState_Setting_ReadMap))
    return;

  Position pos = {
      .x = 0,
      .y = 0,
  };
  Sprite sprite = {.eva_img = RTy(CursorEva), .active = true};
  ScreenCord cord = {
      .z = 2,
  };

  Spawn(MapCursorDisplay, Position, Sprite, ScreenCord, ComponentMarker,
        position_new(pos), sprite_new(sprite), screen_cord_new(cord));
  player_spawn(pos);

  state_set(GameState, GameState_Setting_ReadMap);
}

void read_map() {
  if (!state_is_in(GameState, GameState_Setting_ReadMap))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  // only exist 1 cursor
  QueryIter iter = QueryWith(With(MapCursorDisplay), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER) {
      state_set(GameState, GameState_InGame);
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
    case 'g':
      ghost_spawn(*pos);
      break;
    case 'f':
      break;
    case ' ':
      break;
    }
  }
}

void exit_read_map() {
  if (!state_is_exit(GameState, GameState_Setting_ReadMap))
    return;

  // only exist 1 cursor
  QueryIter iter = QueryEntity(MapCursorDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.query_free(&iter);
  CComponent.despawn(id);
  state_set(GameState, GameState_InGame);
}

void update_number_display() {
  if (!state_is_in(GameState, GameState_Setting_ReadHeight) &&
      !state_is_in(GameState, GameState_Setting_ReadWidth))
    return;

  GameInfo *info = resource_get(GameInfo);

  {
    QueryIter iter = QueryWith(With(HeightDisplay), Number);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    ((Number *)comp[0].self)->n = info->height;
    CComponent.query_free(&iter);
  }
  QueryIter iter = QueryWith(With(WidthDisplay), Number);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  ((Number *)comp[0].self)->n = info->width;
  CComponent.query_free(&iter);
}

void setting_init() {
  add_component_type(MapCursorDisplay);
  add_component_type(HeightDisplay);
  add_component_type(WidthDisplay);

  AddUpdateSystem(update_number_display);
  AddUpdateSystem(exit_read_map, read_map, enter_read_map);
  AddUpdateSystem(exit_read_width, read_width, read_height, enter_read_height);
}
