#include "player.h"
#include "audio.h"
#include "booster.h"
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
  if (!state_is_in(GameState, GameState_InGame))
    return;

  GameInfo info = *resource_get(GameInfo);

  // only exist 1 player
  QueryIter iter =
      QueryWith(With(Player), Position, AnimationCord, Sprite, AnimationSprite);
  PComponent comp[4];
  CComponent.query_next(&iter, array_ref(comp));
  CComponent.query_free(&iter);

  Position *pos = (Position *)(comp[0].self);
  AnimationCord *cord = (AnimationCord *)(comp[1].self);
  Sprite *sprite = (Sprite *)(comp[2].self);
  AnimationSprite *animation_sprite = (AnimationSprite *)(comp[3].self);
  animation_sprite->active = !cord->idle;

  if (!cord->idle)
    return;

  Vec(PEvent) *p_events = event_listen(Key);
  if (p_events->len == 0)
    return;

  Key key = *(Key *)vec_index(PEvent, p_events, 0)->self;

  if (key.kind != Key_Char)
    return;

  switch (key.charater) {
  case 'w':
    if (pos->y == 0)
      return;
    pos->y--;
    sprite->rotation = Rotation_90;
    break;
  case 's':
    if (pos->y + 1 == info.height)
      return;
    pos->y++;
    sprite->rotation = Rotation_270;
    break;
  case 'a':
    if (pos->x == 0)
      return;
    pos->x--;
    sprite->rotation = Rotation_180;
    break;
  case 'd':
    if (pos->x + 1 == info.width)
      return;
    pos->x++;
    sprite->rotation = Rotation_0;
    break;
  default:
    return;
  }

  play_sound(RTy(WalkWav));
  PlayerEvent *event = malloc(sizeof(PlayerEvent));
  *event = PlayerEvent_Moved;
  event_emit(PlayerEvent, CEvent.default_vtable(event));
}

void player_extra_life() {
  if (!state_is_in(GameState, GameState_InGame))
    return;

  GameInfo *info = resource_get(GameInfo);
  Vec(PEvent) *events = event_listen(BoosterEvent);
  for (usize i = 0; i < events->len; i++) {
    BoosterEvent event = *(BoosterEvent *)vec_index(PEvent, events, i)->self;
    if (event != BoosterKind_ExtraLife)
      continue;

    info->life++;
  }
}

void player_spawn(Position pos) {
  Sprite sprite = {
      .eva_img = RTy(PacMan1Eva),
      .active = true,
  };
  ResourceType imgs[8] = {RTy(PacMan1Eva), RTy(PacMan2Eva), RTy(PacMan3Eva),
                          RTy(PacMan4Eva), RTy(PacMan5Eva), RTy(PacMan6Eva),
                          RTy(PacMan7Eva), RTy(PacMan8Eva)};
  Array(ResourceType) array_imgs = array_ref(imgs);
  AnimationSprite animation_sprite = {
      .eva_imgs = array_clone(ResourceType, &array_imgs),
      .loop_mode = LoopMode_LoopInfPingPong,
      .direction = Direction_Forward,
      .ms_per_frame = 20,
      .active = true,
  };
  AnimationCord animation_cord = {
      .ms_per_pixel = PLAYER_ANIMATION_SPEED,
      .active = false,
  };
  ScreenCord cord = {
      .z = 3,
  };
  Spawn(Player, Position, AnimationSprite, Sprite, AnimationCord, ScreenCord,
        ComponentMarker, position_new(pos),
        animation_sprite_new(animation_sprite), sprite_new(sprite),
        animation_cord_new(animation_cord), screen_cord_new(cord));
}

void player_move(Position pos) {
  QueryIter iter = QueryWith(With(Player), Position);
  PComponent comp[1];
  CComponent.query_next(&iter, array_ref(comp));
  *(Position *)comp[0].self = pos;
  CComponent.query_free(&iter);
}

void player_despawn() {
  QueryIter iter = QueryEntity(Player);
  Entity id = *CComponent.query_next(&iter, array_empty(PComponent));
  CComponent.despawn(id);
  CComponent.query_free(&iter);
}

void player_init() {
  add_component_type(Player);
  add_event_type(PlayerEvent);
  AddUpdateSystem(player_move_system, player_extra_life);
}
