#if !defined(GLOBAL_C)
#define GLOBAL_C

#include "global.h"
#include "render.h"
#include <stdlib.h>

DeclareResourceType(GameState);

// Assets
DeclareResourceType(CursorEva);
DeclareResourceType(PacManEva);
DeclareResourceType(BlockEva);

void global_init() {
  add_resource_type(GameState);

  // assets
  add_resource_type(CursorEva);
  resource_insert(CursorEva, open_eva("assets/cursor.eva"));
  add_resource_type(PacManEva);
  resource_insert(PacManEva, open_eva("assets/pac_man.eva"));
  add_resource_type(BlockEva);
  resource_insert(BlockEva, open_eva("assets/block.eva"));

  GameState *state = malloc(sizeof(GameState));
  // state->kind = GameState_InGame;
  *state = GameState_Setting_ReadHeight;
  // *state = GameState_Test;
  resource_insert(GameState, state);
}

#endif // GLOBAL_C