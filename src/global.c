#include "global.h"
#include "input.h"
#include "render.h"
#include <evangel/app.h>
#include <stdlib.h>

DeclareResourceType(GameState);

// Assets
DeclareResourceType(CursorEva);
DeclareResourceType(PacManEva);
DeclareResourceType(BlockEva);

DeclareResourceType(PinkGhostEva);

// Num
DeclareResourceType(Num0);
DeclareResourceType(Num1);
DeclareResourceType(Num2);
DeclareResourceType(Num3);
DeclareResourceType(Num4);
DeclareResourceType(Num5);
DeclareResourceType(Num6);
DeclareResourceType(Num7);
DeclareResourceType(Num8);
DeclareResourceType(Num9);

void exit_system() {
  GameState *game_state = resource_get(GameState);
  Vec(PEvent) *keys = event_listen(Key);
  for (usize i = 0; i < keys->len; i++) {
    Key *key = (Key *)vec_index(PEvent, keys, i)->self;
    if (key->kind != Key_ESC)
      continue;

    if (*game_state == GameState_Menu) {
      CApp.exit();
    }
  }
}

void global_init() {
  add_resource_type(GameState);

  // assets
  add_resource_type(CursorEva);
  add_resource_type(PacManEva);
  add_resource_type(BlockEva);
  resource_insert(CursorEva, open_eva("assets/cursor.eva"));
  resource_insert(PacManEva, open_eva("assets/pac_man.eva"));
  resource_insert(BlockEva, open_eva("assets/block.eva"));

  add_resource_type(PinkGhostEva);
  resource_insert(PinkGhostEva, open_eva("assets/pink_ghost.eva"));

  // num
  add_resource_type(Num0);
  add_resource_type(Num1);
  add_resource_type(Num2);
  add_resource_type(Num3);
  add_resource_type(Num4);
  add_resource_type(Num5);
  add_resource_type(Num6);
  add_resource_type(Num7);
  add_resource_type(Num8);
  add_resource_type(Num9);
  resource_insert(Num0, open_eva("assets/number/num0.eva"));
  resource_insert(Num1, open_eva("assets/number/num1.eva"));
  resource_insert(Num2, open_eva("assets/number/num2.eva"));
  resource_insert(Num3, open_eva("assets/number/num3.eva"));
  resource_insert(Num4, open_eva("assets/number/num4.eva"));
  resource_insert(Num5, open_eva("assets/number/num5.eva"));
  resource_insert(Num6, open_eva("assets/number/num6.eva"));
  resource_insert(Num7, open_eva("assets/number/num7.eva"));
  resource_insert(Num8, open_eva("assets/number/num8.eva"));
  resource_insert(Num9, open_eva("assets/number/num9.eva"));

  GameState *state = malloc(sizeof(GameState));
  // state->kind = GameState_InGame;
  // *state = GameState_Menu;
  *state = GameState_Enter_Setting_ReadHeight;
  // *state = GameState_Test;
  resource_insert(GameState, state);
  AddUpdateSystem(exit_system);
}
