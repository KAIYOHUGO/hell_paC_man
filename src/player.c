#include "player.h"
#include "global.h"
#include "input.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdlib.h>

DeclareComponentType(Player);

DeclareEventType(PlayerEvent);

void player_move_system() {
  GameState state = *resource_get(GameState);
  if (state != GameState_InGame)
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;

  GameInfo info = *resource_get(GameInfo);

  // only exist 1 player
  QueryIter iter = QueryWith(With(Player), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);

  bool exit = false, is_move = false;
  for (usize i = 0; i < p_events->len && !exit; i++) {
    exit = true;
    PEvent *p_event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)p_event->self;

    if (key.kind != Key_Char)
      continue;
    switch (key.charater) {
    case 'w':
      if (pos->y == 0)
        break;
      pos->y--;
      is_move = true;
      break;
    case 's':
      if (pos->y + 1 == info.height)
        break;
      pos->y++;
      is_move = true;
      break;
    case 'a':
      if (pos->x == 0)
        break;
      pos->x--;
      is_move = true;
      break;
    case 'd':
      if (pos->x + 1 == info.width)
        break;
      pos->x++;
      is_move = true;
      break;
    default:
      exit = false;
      break;
    }
  }
  if (is_move) {
    PlayerEvent *event = malloc(sizeof(PlayerEvent));
    *event = PlayerEvent_Moved;
    event_emit(PlayerEvent, CEvent.default_vtable(event));
  }
}

void player_spawn(Position pos) {
  Sprite sprite = {
      .eva_img = RTy(PacManEva),
      .active = true,
  };
  ScreenCord cord = {};
  Spawn(Player, Position, Sprite, ScreenCord, ComponentMarker,
        position_new(pos), sprite_new(sprite), screen_cord_new(cord));
}

void player_move(Position pos) {
  QueryIter iter = QueryWith(With(Player), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  *(Position *)comp[0].self = pos;
  CComponent.query_free(&iter);
}

void player_init() {
  add_component_type(Player);
  add_event_type(PlayerEvent);
  AddUpdateSystem(player_move_system);
}
