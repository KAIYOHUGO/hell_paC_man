#include "state.h"
#include <stdlib.h>

typedef struct StateInfo StateInfo;

static struct StateStorage StateStorage = {};

static StateType raw_add_new_type() {
  usize id = StateStorage.state_list.len;
  StateInfo default_info = {
      .kind = StateKind_In,
      .prev_state = 0,
      .state = 0,
      .next_state = 0,
  };

  vec_push(StateInfo, &StateStorage.state_list, default_info);

  StateType ty = {.id = id};
  return ty;
}

bool raw_is_in(StateType ty, usize state) {
  StateInfo *info = vec_index(StateInfo, &StateStorage.state_list, ty.id);
  return info->kind == StateKind_In && info->state == state;
}

bool raw_is_enter(StateType ty, usize state) {
  StateInfo *info = vec_index(StateInfo, &StateStorage.state_list, ty.id);
  return info->kind == StateKind_Enter && info->state == state;
}

bool raw_is_exit(StateType ty, usize state) {
  StateInfo *info = vec_index(StateInfo, &StateStorage.state_list, ty.id);
  return info->kind == StateKind_Exit && info->prev_state == state;
}

static void raw_set(StateType ty, usize new_state) {
  StateInfo *info = vec_index(StateInfo, &StateStorage.state_list, ty.id);
  info->next_state = new_state;
}

static void raw_flush() {
  for (usize i = 0; i < StateStorage.state_list.len; i++) {
    StateInfo *info = vec_index(StateInfo, &StateStorage.state_list, i);
    switch (info->kind) {
    case StateKind_In:
      if (info->next_state == info->state)
        break;
      info->prev_state = info->state;
      info->state = info->next_state;
      info->kind = StateKind_Exit;
      break;
    case StateKind_Enter:
      info->kind = StateKind_In;
      break;
    case StateKind_Exit:
      info->kind = StateKind_Enter;
      break;
    }
  }
}

void internal_state_storage_init() {
  struct StateStorage storage = {
      .state_list = vec_init(StateInfo),
  };
  StateStorage = storage;
}

const struct CState CState = {
    .add_new_type = raw_add_new_type,
    .is_in = raw_is_in,
    .is_enter = raw_is_enter,
    .is_exit = raw_is_exit,
    .set = raw_set,
    .flush = raw_flush,
};