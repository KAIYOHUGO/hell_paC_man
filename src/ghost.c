#include "ghost.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>

DeclareComponentType(Ghost);

void ghost_move_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Vec(PEvent) *events = event_listen(PlayerEvent);
  if (events->len == 0)
    return;

  GameInfo *info = resource_get(GameInfo);

  QueryIter iter = QueryWith(With(Ghost), Position);
  PComponent comp[1];
  isize directions[5][2] = {{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  for (usize i = 0; i < events->len; i++) {
    PlayerEvent player_event =
        *(PlayerEvent *)vec_index(PEvent, events, i)->self;
    if (player_event != PlayerEvent_Moved)
      continue;

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

    break;
  }
  CComponent.query_free(&iter);
}

void ghost_attack_system() {
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

  QueryIter iter = QueryWith(With(Ghost), Position);
  PComponent comp[1];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Position pos = *(Position *)comp[0].self;
    if (pos.x != player_pos.x || pos.y != player_pos.y)
      continue;

    PlayerEvent *event = malloc(sizeof(PlayerEvent));
    *event = PlayerEvent_Dead;
    event_emit(PlayerEvent, CEvent.default_vtable(event));
  }
  CComponent.query_free(&iter);
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
  AddUpdateSystem(ghost_attack_system, ghost_move_system, ghost_attack_system);
}
