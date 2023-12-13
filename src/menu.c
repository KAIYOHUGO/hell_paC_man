#include "menu.h"
#include "component.h"
#include "global.h"
#include "input.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <evangel/state.h>
#include <stdlib.h>

DeclareComponentType(MenuDisplay);

static void enter_menu() {
  if (!state_is_enter(GameState, GameState_Menu))
    return;

  QueryIter iter = QueryEntity(ScreenCord);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);

  ResourceType imgs[24] = {
      RTy(Menu01Eva), RTy(Menu02Eva), RTy(Menu03Eva), RTy(Menu04Eva),
      RTy(Menu05Eva), RTy(Menu06Eva), RTy(Menu07Eva), RTy(Menu08Eva),
      RTy(Menu09Eva), RTy(Menu10Eva), RTy(Menu11Eva), RTy(Menu12Eva),
      RTy(Menu13Eva), RTy(Menu14Eva), RTy(Menu15Eva), RTy(Menu16Eva),
      RTy(Menu17Eva), RTy(Menu18Eva), RTy(Menu19Eva), RTy(Menu20Eva),
      RTy(Menu21Eva), RTy(Menu22Eva), RTy(Menu23Eva), RTy(Menu24Eva),
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
      .x = 0,
      .y = 0,
  };
  Sprite sprite = {
      .rotation = Rotation_0,
      .active = true,
  };
  Spawn(MenuDisplay, AnimationSprite, ScreenCord, Sprite, ComponentMarker,
        animation_sprite_new(animation_sprite), screen_cord_new(cord),
        sprite_new(sprite));
}

static void read_input() {
  if (!state_is_in(GameState, GameState_Menu))
    return;

  Vec(PEvent) *events = event_listen(Key);
  for (usize i = 0; i < events->len; i++) {
    Key key = *(Key *)vec_index(PEvent, events, i)->self;
    switch (key.kind) {
    case Key_ENTER:
      state_set(GameState, GameState_Setting_ReadHeight);
      return;
    case Key_ESC:
      CApp.exit();
      return;
    default:
      continue;
    }
  }
}

static void exit_menu() {
  if (!state_is_exit(GameState, GameState_Menu))
    return;

  QueryIter iter = QueryEntity(MenuDisplay);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.despawn(id);
  CComponent.query_free(&iter);
}

void menu_init() {
  add_component_type(MenuDisplay);
  AddUpdateSystem(enter_menu, read_input, exit_menu);
}