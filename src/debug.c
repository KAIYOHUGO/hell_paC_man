#if !defined(DEBUG_H)
#define DEBUG_H

#include "debug.h"
#include "component.h"
#include "player.h"
#include <evangel/component.h>
#include <stdio.h>

void sprite_debug() {
  QueryIter iter = QueryWith(With(Player), Position);

  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);
  Position *pos = (Position *)(comp[0].self);

  printf("Debug> Player Pos:(%d %d)\n", pos->x, pos->y);
}

#endif // DEBUG_H