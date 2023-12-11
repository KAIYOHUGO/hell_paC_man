#include "lose.h"
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
#include <evangel/event.h>

DeclareComponentType(LoseDisplay);

static void enter_lose() {
  if (!state_is_enter(GameState, GameState_Lose))
    return;

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

  ResourceType imgs[17] = {
      RTy(LoseMenu01Eva), RTy(LoseMenu02Eva), RTy(LoseMenu03Eva),
      RTy(LoseMenu04Eva), RTy(LoseMenu05Eva), RTy(LoseMenu06Eva),
      RTy(LoseMenu07Eva), RTy(LoseMenu08Eva), RTy(LoseMenu09Eva),
      RTy(LoseMenu10Eva), RTy(LoseMenu11Eva), RTy(LoseMenu12Eva),
      RTy(LoseMenu13Eva), RTy(LoseMenu14Eva), RTy(LoseMenu15Eva),
      RTy(LoseMenu16Eva), RTy(LoseMenu17Eva),
  };

  Array(ResourceType) array_imgs = array_ref(imgs);
  AnimationSprite animation_sprite = {
      .eva_imgs = array_clone(ResourceType, &array_imgs),
      .loop_mode = LoopMode_LoopInf,
      .direction = Direction_Forward,
      .ms_per_frame = 60,
      .active = true,
  };
  ScreenCord cord = {
      .z = 3,
      .x = (SCREEN_WIDTH - LOSE_WIDTH) / 2,
      .y = (SCREEN_HIGHT - LOSE_HEIGHT) / 2,
  };
  Sprite sprite = {
      .rotation = Rotation_0,
      .active = true,
  };
  Spawn(LoseDisplay, AnimationSprite, ScreenCord, Sprite, ComponentMarker,
        animation_sprite_new(animation_sprite), screen_cord_new(cord),
        sprite_new(sprite));

  play_sound(RTy(DeathWav));
}

static void read_input() {
  if (!state_is_in(GameState, GameState_Lose))
    return;

  Vec(PEvent) *events = event_listen(Key);
  for (usize i = 0; i < events->len; i++) {
    Key key = *(Key *)vec_index(PEvent, events, i)->self;
    switch (key.kind) {
    case Key_ENTER:
      state_set(GameState, GameState_GenerateMap);
      return;
    case Key_ESC:
      state_set(GameState, GameState_Menu);
      return;
    default:
      continue;
    }
  }
}

static void exit_lose() {
  if (!state_is_exit(GameState, GameState_Lose))
    return;

  food_despawn();
  ghost_despawn();
  booster_despawn();
  player_despawn();
  in_game_display_despawn();

  QueryIter iter = QueryEntity(LoseDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.despawn(id);
  CComponent.query_free(&iter);
}

void lose_init() {
  add_component_type(LoseDisplay);
  AddUpdateSystem(enter_lose, read_input, exit_lose);
}