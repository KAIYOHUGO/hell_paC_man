#include "ghost.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>

DeclareComponentType(Ghost);

void ghost_move_system() {
  GameState game_state = *resource_get(GameState);
  if (game_state != GameState_InGame)
    return;

  Vec(PEvent) *events = event_listen(PlayerEvent);
  if (events->len == 0)
    return;

  GameInfo *info = resource_get(GameInfo);

  QueryIter iter = QueryWith(With(Ghost), Position);
  PComponent comp[1];
  isize directions[5][2] = {{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Position *pos = (Position *)comp[0].self;
    isize *direction = directions[rand() % 5];
    if (0 <= pos->x + direction[0] && pos->x + direction[0] < info->width) {
      pos->x += direction[0];
    }
    if (0 <= pos->y + direction[1] && pos->y + direction[1] < info->height) {
      pos->y += direction[1];
    }
  }
}

void ghost_spawn(Position pos) {
  Sprite sprite = {
      .eva_img = RTy(PinkGhostEva),
      .active = true,
  };
  ScreenCord cord = {};
  Spawn(Ghost, Position, Sprite, ScreenCord, ComponentMarker, position_new(pos),
        sprite_new(sprite), screen_cord_new(cord));
}

void ghost_init() {
  add_component_type(Ghost);
  AddUpdateSystem(ghost_move_system)
}
