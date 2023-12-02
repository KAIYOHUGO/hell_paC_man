#if !defined(DEBUG_H)
#define DEBUG_H

#include "debug.h"
#include "component.h"
#include "input.h"
#include "player.h"
#include "state.h"
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

void setting_debug() {
  GameInfo *info = resource_get(GameInfo);
  printf("Map Size (%u x %u)\n", info->height, info->width);
  if (info->map.ptr == NULL)
    return;
  for (usize i = 0; i < info->height; i++) {
    for (usize j = 0; j < info->width; j++) {
      MapItem item = *array_index(MapItem, &info->map, i * info->width + j);
      printf("%u ", item);
    }
    printf("\n");
  }
}

#endif // DEBUG_H