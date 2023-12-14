#include "ghost.h"
#include "audio.h"
#include "booster.h"
#include "global.h"
#include "player.h"
#include <evangel/app.h>
#include <evangel/event.h>
#include <stdlib.h>

DeclareComponentType(Ghost);

static ResourceType get_ghost_img(GhostKind kind) {
  ResourceType variant[4] = {RTy(BlueGhostEva), RTy(PinkGhostEva),
                             RTy(RedGhostEva), RTy(YellowGhostEva)};
  return variant[(usize)kind];
}

static ResourceType get_ghost_freeze_img(GhostKind kind) {
  ResourceType variant[4] = {RTy(BlueGhostFreezeEva), RTy(PinkGhostFreezeEva),
                             RTy(RedGhostFreezeEva), RTy(YellowGhostFreezeEva)};
  return variant[(usize)kind];
}

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

    QueryIter iter = Query(Ghost, Position, Sprite);
    PComponent comp[3];
    while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
      Ghost *ghost = (Ghost *)comp[0].self;
      Position *pos = (Position *)comp[1].self;
      Sprite *sprite = (Sprite *)comp[2].self;

      if (ghost->dead)
        continue;

      if (ghost->cd != 0) {
        ghost->cd--;
        sprite->eva_img = get_ghost_freeze_img(ghost->kind);
        continue;
      }

      sprite->eva_img = get_ghost_img(ghost->kind);

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

  QueryIter iter = Query(Ghost, ScreenCord);
  PComponent comp[2];
  while (CComponent.query_next(&iter, array_ref(comp)) != NULL) {
    Ghost *ghost = (Ghost *)comp[0].self;
    ScreenCord cord = *(ScreenCord *)comp[1].self;
    if (ghost->dead)
      continue;

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
      Ghost *ghost = (Ghost *)comp[0].self;
      ghost->cd = 3;
    }
    CComponent.query_free(&iter);
  }
}

static void kill_ghost(Entity entity) {
  PComponent comp[2];
  bool ok = GetComponent(entity, array_ref(comp), Ghost, AnimationSprite);
  if (!ok)
    return;

  ResourceType imgs[8] = {
      RTy(GhostDie1Eva), RTy(GhostDie2Eva), RTy(GhostDie3Eva),
      RTy(GhostDie4Eva), RTy(GhostDie5Eva), RTy(GhostDie6Eva),
  };
  Array(ResourceType) array_imgs = array_ref(imgs);
  Ghost *ghost = (Ghost *)comp[0].self;
  AnimationSprite *sprite = (AnimationSprite *)comp[1].self;

  ghost->dead = true;
  sprite->eva_imgs = array_clone(ResourceType, &array_imgs);
  sprite->loop_mode = LoopMode_Disable;
  sprite->ms_per_frame = 150;
  sprite->active = true;
  play_sound(RTy(GhostDeadWav));
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

    Vec(Entity) entities = vec_init(Entity);
    QueryIter iter = QueryEntity(Ghost);
    Entity *id;
    while ((id = CComponent.query_next(&iter, array_empty(PComponent))) !=
           NULL) {
      vec_push(Entity, &entities, *id);
    }
    CComponent.query_free(&iter);
    if (entities.len == 0)
      return;

    Entity random_entity = *vec_index(Entity, &entities, rand() % entities.len);
    kill_ghost(random_entity);
  }
}

static void ghost_dead_animation() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  QueryIter iter = Query(Ghost, AnimationSprite);
  PComponent comp[2];
  Entity *id;
  while ((id = CComponent.query_next(&iter, array_ref(comp))) != NULL) {
    Ghost *ghost = (Ghost *)comp[0].self;
    AnimationSprite *sprite = (AnimationSprite *)comp[1].self;
    if (!ghost->dead || sprite->active)
      continue;

    CComponent.despawn(*id);
  }
}

PComponent ghost_new(Ghost ghost) {
  Ghost *ptr = malloc(sizeof(Ghost));
  *ptr = ghost;
  return CComponent.default_vtable(ptr);
}

void ghost_spawn(Position pos) {
  GhostKind kind = rand() % _GhostKind_Size;
  Sprite sprite = {
      .eva_img = get_ghost_img(kind),
      .active = true,
  };
  AnimationCord animation_cord = {
      .ms_per_pixel = PLAYER_ANIMATION_SPEED,
      .active = true,
  };
  ScreenCord cord = {
      .z = 2,
  };
  Ghost ghost = {.cd = 0, .kind = kind};
  AnimationSprite animation_sprite = {.active = false};
  Spawn(Ghost, Position, AnimationSprite, Sprite, AnimationCord, ScreenCord,
        ghost_new(ghost), position_new(pos),
        animation_sprite_new(animation_sprite), sprite_new(sprite),
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
  AddUpdateSystem(freeze_booster, less_ghost_booster, ghost_dead_animation);
  AddUpdateSystem(ghost_move_system, ghost_attack_system);
}
