#include "setting.h"
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
#include <stdlib.h>

#define GAME_MODE_Y ((SCREEN_HIGHT - NUMBER_SIZE * 5) / 2)
#define HEIGHT_DISPLAY_Y ((SCREEN_HIGHT - NUMBER_SIZE) / 2)
#define WIDTH_DISPLAY_Y (SCREEN_HIGHT + NUMBER_SIZE * 3) / 2

DeclareComponentType(ArrowDisplay);
DeclareComponentType(MapCursorDisplay);
DeclareComponentType(ModeDisplay);
DeclareComponentType(HeightDisplay);
DeclareComponentType(WidthDisplay);
DeclareComponentType(BackgroundBlock);
DeclareComponentType(BackgroundText);

static void enter_read_mode_system() {
  if (!state_is_enter(GameState, GameState_Setting_ReadMode))
    return;

  GameInfo *info = resource_get(GameInfo);
  info->height = 0;
  info->width = 0;
  info->life = 1;
  info->mode = GameMode_Default;

  {
    Sprite sprite = {
        .active = true,
        .eva_img = RTy(SettingEva),
    };
    ScreenCord cord = {.x = SCREEN_WIDTH / 2 - SETTING_WIDTH,
                       .y = (SCREEN_HIGHT - SETTING_HEIGHT) / 2,
                       .z = 0};
    Spawn(BackgroundText, Sprite, ScreenCord, ComponentMarker,
          sprite_new(sprite), screen_cord_new(cord));
  }
  {
    Sprite sprite = {
        .active = true,
        .eva_img = RTy(GameModeDefaultEva),
    };
    ScreenCord cord = {.x = SCREEN_WIDTH / 2, .y = GAME_MODE_Y, .z = 1};
    Spawn(ModeDisplay, Sprite, ScreenCord, ComponentMarker, sprite_new(sprite),
          screen_cord_new(cord));
  }
  {
    Sprite sprite = {
        .eva_img = RTy(Arrow1Eva),
        .active = true,
    };
    ResourceType imgs[5] = {
        RTy(Arrow1Eva), RTy(Arrow2Eva), RTy(Arrow3Eva),
        RTy(Arrow4Eva), RTy(Arrow5Eva),
    };
    Array(ResourceType) array_imgs = array_ref(imgs);
    AnimationSprite animation_sprite = {
        .eva_imgs = array_clone(ResourceType, &array_imgs),
        .loop_mode = LoopMode_LoopInfPingPong,
        .direction = Direction_Forward,
        .ms_per_frame = 150,
        .active = true,
    };
    ScreenCord cord = {
        .x = SCREEN_WIDTH / 2 - SETTING_WIDTH - SETTING_ARROW_SIZE,
        .y = GAME_MODE_Y,
        .z = 3,
    };
    AnimationCord animation_cord = {
        .x = cord.x,
        .y = cord.y,
        .ms_per_pixel = 20,
        .active = true,
    };
    Spawn(ArrowDisplay, AnimationSprite, Sprite, AnimationCord, ScreenCord,
          ComponentMarker, animation_sprite_new(animation_sprite),
          sprite_new(sprite), animation_cord_new(animation_cord),
          screen_cord_new(cord));
  }
  {
    Number num = {
        .len = 2,
    };
    ScreenCord cord = {.x = SCREEN_WIDTH / 2, .y = HEIGHT_DISPLAY_Y, .z = 1};
    Spawn(HeightDisplay, Number, ScreenCord, ComponentMarker, number_new(num),
          screen_cord_new(cord));
  }
  {
    Number num = {
        .n = 0,
        .len = 2,
    };
    ScreenCord cord = {.x = SCREEN_WIDTH / 2, .y = WIDTH_DISPLAY_Y, .z = 1};
    Spawn(WidthDisplay, Number, ScreenCord, ComponentMarker, number_new(num),
          screen_cord_new(cord));
  }
}

static void read_mode_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadMode))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER) {
      play_sound(RTy(SelectWav));
      state_set(GameState, GameState_Setting_ReadHeight);
      return;
    }

    if (key.kind != Key_Char)
      continue;

    switch (key.charater) {
    case 'w':
      play_sound(RTy(SelectWav));
      info->mode = (info->mode + _GameMode_Size - 1) % _GameMode_Size;
      break;
    case 's':
    case ' ':
      play_sound(RTy(SelectWav));
      info->mode = (info->mode + _GameMode_Size + 1) % _GameMode_Size;
      break;
    }
  }
}

static void enter_read_height_system() {
  if (!state_is_enter(GameState, GameState_Setting_ReadHeight))
    return;

  QueryIter iter = QueryWith(With(ArrowDisplay), AnimationCord);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);
  ((AnimationCord *)(comp[0].self))->y = HEIGHT_DISPLAY_Y;
}

static void read_height_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadHeight))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER && info->height >= 3) {
      play_sound(RTy(SelectWav));
      state_set(GameState, GameState_Setting_ReadWidth);
      return;
    }

    if (key.kind == Key_Backspace) {
      play_sound(RTy(SelectWav));
      info->height /= 10;
      continue;
    }

    if (key.kind != Key_Number || info->height > 9)
      continue;
    play_sound(RTy(SelectWav));
    info->height *= 10;
    info->height += key.number;
  }
}

static void enter_read_width_system() {
  if (!state_is_enter(GameState, GameState_Setting_ReadWidth))
    return;

  QueryIter iter = QueryWith(With(ArrowDisplay), AnimationCord);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);
  ((AnimationCord *)(comp[0].self))->y = WIDTH_DISPLAY_Y;
}

static void read_width_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadWidth))
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;
  GameInfo *info = resource_get(GameInfo);

  for (usize i = 0; i < p_events->len; i++) {
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind == Key_ENTER && info->width >= 3) {
      play_sound(RTy(SelectWav));
      switch (info->mode) {
      case GameMode_Custom:
        state_set(GameState, GameState_Setting_ReadMap);
        break;
      case GameMode_Default:
        state_set(GameState, GameState_GenerateMap);
        break;
      default:
        break;
      }
      return;
    }

    if (key.kind == Key_Backspace) {
      play_sound(RTy(SelectWav));
      info->width /= 10;
      continue;
    }

    if (key.kind != Key_Number || info->width > 9)
      continue;
    play_sound(RTy(SelectWav));
    info->width *= 10;
    info->width += key.number;
  }
}

static void exit_read_width_system() {
  if (!state_is_exit(GameState, GameState_Setting_ReadWidth))
    return;

  // clean stage
  QueryIter iter = QueryEntity(ScreenCord);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);

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
      Spawn(BackgroundBlock, Sprite, Position, ScreenCord, ComponentMarker,
            sprite_new(block_sprite), position_new(pos),
            screen_cord_new(block_cord));
    }
  }
}

static void enter_read_map_system() {
  if (!state_is_enter(GameState, GameState_Setting_ReadMap))
    return;

  GameInfo *info = resource_get(GameInfo);
  info->food_amount = 0;
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

static void despawn_at_pos(Position pos) {
  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = QueryWithout(
      Without(Player, BackgroundBlock, MapCursorDisplay), Position);
  PComponent comp[1];
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {
    Position comp_pos = *(Position *)comp[0].self;
    if (comp_pos.x != pos.x || comp_pos.y != pos.y)
      continue;

    if (GetComponent(*id, array_empty(PComponent), Food))
      info->food_amount--;

    CComponent.despawn(*id);
    break;
  }
}

static void read_map_system() {
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
      play_sound(RTy(SelectWav));
      state_set(GameState, GameState_InGame);
      return;
    }

    if (key.kind != Key_Char)
      continue;

    switch (key.charater) {
    case 'w':
      if (pos->y == 0)
        break;
      play_sound(RTy(SelectWav));
      pos->y--;
      break;
    case 's':
      if (pos->y + 1 == info->height)
        break;
      play_sound(RTy(SelectWav));
      pos->y++;
      break;
    case 'a':
      if (pos->x == 0)
        break;
      play_sound(RTy(SelectWav));
      pos->x--;
      break;
    case 'd':
      if (pos->x + 1 == info->width)
        break;
      play_sound(RTy(SelectWav));
      pos->x++;
      break;
    case 'p':
      play_sound(RTy(SelectWav));
      player_move(*pos);
      break;
    case 'b':
      play_sound(RTy(SelectWav));
      booster_spawn(*pos);
      break;
    case 'g':
      play_sound(RTy(SelectWav));
      ghost_spawn(*pos);
      break;
    case 'f':
      play_sound(RTy(SelectWav));
      food_spawn(*pos);
      info->food_amount++;
      break;
    case ' ':
      play_sound(RTy(SelectWav));
      despawn_at_pos(*pos);
      break;
    }
  }
}

static void exit_read_map_system() {
  if (!state_is_exit(GameState, GameState_Setting_ReadMap))
    return;

  // only exist 1 cursor
  QueryIter iter = QueryEntity(MapCursorDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.query_free(&iter);
  CComponent.despawn(id);
}

static void update_mode_display_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadMode))
    return;

  ResourceType list[2] = {RTy(GameModeDefaultEva), RTy(GameModeCustomEva)};
  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = QueryWith(With(ModeDisplay), Sprite);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  ((Sprite *)comp[0].self)->eva_img = list[(usize)info->mode];
  CComponent.query_free(&iter);
}

static void update_height_number_display_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadHeight))
    return;

  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = QueryWith(With(HeightDisplay), Number);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  ((Number *)comp[0].self)->n = info->height;
  CComponent.query_free(&iter);
}

static void update_width_number_display_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadWidth))
    return;

  GameInfo *info = resource_get(GameInfo);
  QueryIter iter = QueryWith(With(WidthDisplay), Number);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  ((Number *)comp[0].self)->n = info->width;
  CComponent.query_free(&iter);
}

static void back_to_menu_system() {
  if (!state_is_in(GameState, GameState_Setting_ReadMode) &&
      !state_is_in(GameState, GameState_Setting_ReadHeight) &&
      !state_is_in(GameState, GameState_Setting_ReadWidth) &&
      !state_is_in(GameState, GameState_Setting_ReadMap))
    return;

  Vec(PEvent) *events = event_listen(Key);
  for (usize i = 0; i < events->len; i++) {
    Key key = *(Key *)vec_index(PEvent, events, i)->self;
    if (key.kind != Key_ESC)
      continue;

    state_set(GameState, GameState_Menu);
  }
}

void setting_init() {
  add_component_type(ArrowDisplay);
  add_component_type(MapCursorDisplay);
  add_component_type(ModeDisplay);
  add_component_type(HeightDisplay);
  add_component_type(WidthDisplay);
  add_component_type(BackgroundBlock);
  add_component_type(BackgroundText);

  AddUpdateSystem(update_mode_display_system,
                  update_height_number_display_system,
                  update_width_number_display_system, back_to_menu_system);
  AddUpdateSystem(enter_read_mode_system, read_mode_system);
  AddUpdateSystem(enter_read_height_system, read_height_system,
                  enter_read_width_system, read_width_system,
                  exit_read_width_system);
  AddUpdateSystem(enter_read_map_system, read_map_system, exit_read_map_system);
}
