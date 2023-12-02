#if !defined(STATE_C)
#define STATE_C

#include <stdlib.h>
#include "state.h"

DeclareResourceType(GameState);

void state_init() {
  add_resource_type(GameState);

  GameState *state = malloc(sizeof(GameState));
  // state->kind = GameState_InGame;
  *state = GameState_Setting_ReadHeight;
  resource_insert(GameState, state);
}

#endif // STATE_C