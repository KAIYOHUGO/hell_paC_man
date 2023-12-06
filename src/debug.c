#include "debug.h"
#include "component.h"
#include "input.h"
#include "player.h"
#include "global.h"
#include <evangel/component.h>
#include <evangel/event.h>
#include <evangel/resource.h>
#include <stdio.h>

void sprite_debug() {
  QueryIter iter = Query(Position);

  PComponent comp[1];
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {

    Position *pos = (Position *)(comp[0].self);

    printf("Debug> Entity{%d} Pos:(%d %d)\n", id->id, pos->x, pos->y);
  }
  CComponent.query_free(&iter);
}

void state_debug() {
  GameState state = *resource_get(GameState);
  printf("state: %u\n", state);
}
