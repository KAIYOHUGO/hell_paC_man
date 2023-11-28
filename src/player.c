#if !defined(PLAYER_C)
#define PLAYER_C

#include "player.h"
#include "component.h"
#include "input.h"
#include "state.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdlib.h>

DeclareComponentType(Player);

PComponent player_new() {
  Player *ptr = malloc(sizeof(Player));
  Player player = {};
  *ptr = player;
  return CComponent.default_vtable(ptr);
}

void player_move_system() {
  GameState state = *resource_get(GameState);
  if (state != GameState_InGame)
    return;
  
  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;

  QueryIter iter = QueryWith(With(Player), Position);

  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);

  bool exit = false;
  for (usize i = 0; i < p_events->len && !exit; i++) {
    exit = true;
    PEvent *event = vec_index(PEvent, p_events, i);
    Key key = *(Key *)event->self;
    switch (key) {
    case Key_W:
      if (pos->y == 0)
        break;
      pos->y--;
      break;
    case Key_S:
      if (pos->y + 1 == 10)
        break;
      pos->y++;
      break;
    case Key_A:
      if (pos->x == 0)
        break;
      pos->x--;
      break;
    case Key_D:
      if (pos->x + 1 == 10)
        break;
      pos->x++;
      break;
    default:
      exit = false;
      break;
    }
  }
}

void player_init() {
  CApp.add_update_system(player_move_system);
  Position pos = {.x = 1, .y = 1};
  Spawn(Player, Position, player_new(), position_new(pos));
}

#endif // PLAYER_C