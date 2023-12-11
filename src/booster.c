#include "booster.h"
#include "audio.h"
#include "global.h"
#include "player.h"
#include <evangel/state.h>
#include <stdlib.h>

DeclareComponentType(Booster);
DeclareEventType(BoosterEvent);

void player_get_booster_system() {
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

  QueryIter iter = QueryWith(With(Booster), Position);
  PComponent comp[1];
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {
    Position pos = *(Position *)comp[0].self;
    if (pos.x != player_pos.x || pos.y != player_pos.y)
      continue;

    BoosterEvent *event = malloc(sizeof(BoosterEvent));
    *event = (BoosterEvent)(rand() % _BoosterKind_End);
    // *event = BoosterKind_LessGhost;
    event_emit(BoosterEvent, CEvent.default_vtable(event));
    CComponent.despawn(*id);
    play_sound(RTy(BoosterWav));
  }
  CComponent.query_free(&iter);
}

void booster_spawn(Position pos) {
  Sprite sprite = {
      .eva_img = RTy(BoosterEva),
      .active = true,
  };
  ScreenCord cord = {};
  Spawn(Booster, Position, Sprite, ScreenCord, ComponentMarker,
        position_new(pos), sprite_new(sprite), screen_cord_new(cord));
}

void booster_despawn() {
  QueryIter iter = QueryEntity(Booster);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void booster_init() {
  add_component_type(Booster);
  add_event_type(BoosterEvent);
  AddUpdateSystem(player_get_booster_system);
}