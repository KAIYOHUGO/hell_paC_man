#if !defined(STATE_C)
#define STATE_C

#include "state.h"
#include <stdlib.h>

DeclareResourceType(GameState);

void state_init() {
  add_resource_type(GameState);

  GameState *state = malloc(sizeof(GameState));
  // state->kind = GameState_InGame;
  // *state = GameState_Setting_ReadHeight;
  *state = GameState_Test;
  resource_insert(GameState, state);
}

#endif // STATE_C