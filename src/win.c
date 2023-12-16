#include "win.h"
#include "audio.h"
#include "booster.h"
#include "component.h"
#include "food.h"
#include "ghost.h"
#include "global.h"
#include "in_game.h"
#include "input.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/resource.h>
#include <evangel/state.h>

DeclareComponentType(WinDisplay);

static void enter_win_system() {
  if (!state_is_enter(GameState, GameState_Win))
    return;

  ResourceType imgs[21] = {
      RTy(WinMenu01Eva), RTy(WinMenu02Eva), RTy(WinMenu03Eva),
      RTy(WinMenu04Eva), RTy(WinMenu05Eva), RTy(WinMenu06Eva),
      RTy(WinMenu07Eva), RTy(WinMenu08Eva), RTy(WinMenu09Eva),
      RTy(WinMenu10Eva), RTy(WinMenu11Eva), RTy(WinMenu12Eva),
      RTy(WinMenu13Eva), RTy(WinMenu14Eva), RTy(WinMenu15Eva),
      RTy(WinMenu16Eva), RTy(WinMenu17Eva), RTy(WinMenu18Eva),
      RTy(WinMenu19Eva), RTy(WinMenu20Eva), RTy(WinMenu21Eva),
  };
  Array(ResourceType) array_imgs = array_ref(imgs);
  AnimationSprite animation_sprite = {
      .eva_imgs = array_clone(ResourceType, &array_imgs),
      .loop_mode = LoopMode_LoopInf,
      .direction = Direction_Forward,
      .ms_per_frame = 40,
      .active = true,
  };
  ScreenCord cord = {
      .z = 3,
      .x = (SCREEN_WIDTH - WIN_WIDTH) / 2,
      .y = (SCREEN_HIGHT - WIN_HEIGHT) / 2,
  };
  Sprite sprite = {
      .rotation = Rotation_0,
      .active = true,
  };
  Spawn(WinDisplay, AnimationSprite, ScreenCord, Sprite, ComponentMarker,
        animation_sprite_new(animation_sprite), screen_cord_new(cord),
        sprite_new(sprite));
}

static void read_input_system() {
  if (!state_is_in(GameState, GameState_Win))
    return;

  GameInfo *info = resource_get(GameInfo);
  Vec(PEvent) *events = event_listen(Key);
  for (usize i = 0; i < events->len; i++) {
    Key key = *(Key *)vec_index(PEvent, events, i)->self;
    switch (key.kind) {
    case Key_ENTER:
      switch (info->mode) {
      case GameMode_Default:
        state_set(GameState, GameState_GenerateMap);
        break;
      case GameMode_Custom:
        state_set(GameState, GameState_Setting_ReadMap);
        break;
      default:
        break;
      }
      return;
    case Key_ESC:
      state_set(GameState, GameState_Menu);
      return;
    default:
      continue;
    }
  }
}

static void exit_win_system() {
  if (!state_is_exit(GameState, GameState_Win))
    return;

  play_sound(RTy(SelectWav));

  GameInfo *info = resource_get(GameInfo);
  info->life = 1;
  food_despawn();
  ghost_despawn();
  booster_despawn();
  player_despawn();
  in_game_display_despawn();

  QueryIter iter = QueryEntity(WinDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.despawn(id);
  CComponent.query_free(&iter);
}

void win_init() {
  add_component_type(WinDisplay);
  AddUpdateSystem(enter_win_system, read_input_system, exit_win_system);
}