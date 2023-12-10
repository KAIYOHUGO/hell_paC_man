#include "lose.h"
#include "audio.h"
#include "component.h"
#include "global.h"
#include "input.h"
#include <evangel/app.h>
#include <evangel/event.h>

DeclareComponentType(LoseDisplay);

void enter_lose() {
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
  QueryIter iter = Query(AnimationSprite);
  PComponent comp[1];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    AnimationSprite *animation_sprite = (AnimationSprite *)comp[0].self;
    // TODO: add lose animation
    animation_sprite->active = false;
  }
  CComponent.query_free(&iter);

  play_sound(RTy(DeathWav));
}

void lose_menu() {
  if (!state_is_in(GameState, GameState_Lose))
    return;

  Vec(PEvent) *keys = event_listen(Key);
  for (usize i = 0; i < keys->len; i++) {
    Key key = *(Key *)vec_index(PEvent, keys, i)->self;
    if (key.kind != Key_ENTER)
      continue;

    state_set(GameState, GameState_Menu);
    break;
  }
}

void lose_init() {
  add_component_type(LoseDisplay);
  AddUpdateSystem(enter_lose, lose_menu);
}