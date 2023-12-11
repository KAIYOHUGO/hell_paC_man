#include "ghost.h"
#include "booster.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <stdlib.h>

DeclareComponentType(Ghost);
static void ghost_move_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Vec(PEvent) *events = event_listen(PlayerEvent);
  if (events->len == 0)
    return;

  GameInfo *info = resource_get(GameInfo);

  isize directions[5][2] = {{0, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  for (usize i = 0; i < events->len; i++) {
    PlayerEvent player_event =
        *(PlayerEvent *)vec_index(PEvent, events, i)->self;
    if (player_event != PlayerEvent_Moved)
      continue;

    QueryIter iter = Query(Ghost, Position);
    PComponent comp[2];
    while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
      Ghost *ghost = (Ghost *)comp[0].self;
      Position *pos = (Position *)comp[1].self;

      if (ghost->cd != 0) {
        ghost->cd--;
        continue;
      }

      isize *direction = directions[rand() % 5];
      if (0 <= pos->x + direction[0] && pos->x + direction[0] < info->width) {
        pos->x += direction[0];
      }
      if (0 <= pos->y + direction[1] && pos->y + direction[1] < info->height) {
        pos->y += direction[1];
      }
    }
    CComponent.query_free(&iter);

    break;
  }
}

static void ghost_attack_system() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  ScreenCord player_cord;
  {
    // only exist 1 player
    QueryIter iter = QueryWith(With(Player), ScreenCord);
    PComponent comp[1];
    CComponent.query_next(&iter, array_ref(comp));
    player_cord = *(ScreenCord *)comp[0].self;
    CComponent.query_free(&iter);
  }

  QueryIter iter = QueryWith(With(Ghost), ScreenCord);
  PComponent comp[1];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    ScreenCord cord = *(ScreenCord *)comp[0].self;
    isize d_x = player_cord.x - cord.x, d_y = player_cord.y - cord.y;
    if (d_x * d_x + d_y * d_y > 3)
      continue;

    PlayerEvent *event = malloc(sizeof(PlayerEvent));
    *event = PlayerEvent_Die;
    event_emit(PlayerEvent, CEvent.default_vtable(event));
  }
  CComponent.query_free(&iter);
}

static void freeze_booster() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Vec(PEvent) *events = event_listen(BoosterEvent);
  if (events->len == 0)
    return;

  for (usize i = 0; i < events->len; i++) {
    BoosterEvent event = *(BoosterEvent *)vec_index(PEvent, events, i)->self;
    if (event != BoosterKind_Freeze)
      continue;

    QueryIter iter = Query(Ghost);
    PComponent comp[1];
    while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
      ((Ghost *)comp[0].self)->cd = 3;
    }
    CComponent.query_free(&iter);
  }
}

static void less_ghost_booster() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  Vec(PEvent) *events = event_listen(BoosterEvent);
  if (events->len == 0)
    return;

  for (usize i = 0; i < events->len; i++) {
    BoosterEvent event = *(BoosterEvent *)vec_index(PEvent, events, i)->self;
    if (event != BoosterKind_LessGhost)
      continue;

    QueryIter iter = QueryEntity(Ghost);
    Entity *id;
    Vec(Entity) ids = vec_init(Entity);
    while ((id = CComponent.query_next(&iter, array_empty(PComponent))) !=
           NULL) {
      vec_push(Entity, &ids, *id);
    }
    CComponent.query_free(&iter);
    if (ids.len == 0)
      return;

    Entity random_id = *vec_index(Entity, &ids, rand() % ids.len);
    vec_free(Entity, &ids);

    CComponent.despawn(random_id);
  }
}

PComponent ghost_new(Ghost ghost) {
  Ghost *ptr = malloc(sizeof(Ghost));
  *ptr = ghost;
  return CComponent.default_vtable(ptr);
}

void ghost_spawn(Position pos) {
  ResourceType variant[4] = {RTy(PinkGhostEva), RTy(RedGhostEva),
                             RTy(BlueGhostEva), RTy(YellowGhostEva)};
  Sprite sprite = {
      .eva_img = variant[rand() % 4],
      .active = true,
  };
  AnimationCord animation_cord = {
      .ms_per_pixel = PLAYER_ANIMATION_SPEED,
      .active = true,
  };
  ScreenCord cord = {
      .z = 2,
  };
  Ghost ghost = {
      .cd = 0,
  };
  Spawn(Ghost, Position, Sprite, AnimationCord, ScreenCord, ghost_new(ghost),
        position_new(pos), sprite_new(sprite),
        animation_cord_new(animation_cord), screen_cord_new(cord));
}

void ghost_despawn() {
  QueryIter iter = QueryEntity(Ghost);
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_empty(PComponent))) != NULL) {
    CComponent.despawn(*id);
  }
  CComponent.query_free(&iter);
}

void ghost_init() {
  add_component_type(Ghost);
  AddUpdateSystem(ghost_move_system, ghost_attack_system);
  AddUpdateSystem(freeze_booster, less_ghost_booster);
}
