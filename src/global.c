#include "global.h"
#include "audio.h"
#include "input.h"
#include "render.h"
#include <evangel/app.h>
#include <stdlib.h>

DeclareStateType(GameState);

DeclareResourceType(GameInfo);

// Audio
DeclareResourceType(BeginningWav);
DeclareResourceType(DeathWav);
DeclareResourceType(EatWav);
DeclareResourceType(WalkWav);
DeclareResourceType(SirenWav);
DeclareResourceType(BoosterWav);

// Assets
DeclareResourceType(CursorEva);
DeclareResourceType(PacManEva);
DeclareResourceType(BlockEva);
DeclareResourceType(CherryEva);
DeclareResourceType(BoosterEva);
// Pac man
DeclareResourceType(PacMan1Eva);
DeclareResourceType(PacMan2Eva);
DeclareResourceType(PacMan3Eva);
DeclareResourceType(PacMan4Eva);
DeclareResourceType(PacMan5Eva);
DeclareResourceType(PacMan6Eva);
DeclareResourceType(PacMan7Eva);
DeclareResourceType(PacMan8Eva);
// win menu
DeclareResourceType(WinMenu01Eva);
DeclareResourceType(WinMenu02Eva);
DeclareResourceType(WinMenu03Eva);
DeclareResourceType(WinMenu04Eva);
DeclareResourceType(WinMenu05Eva);
DeclareResourceType(WinMenu06Eva);
DeclareResourceType(WinMenu07Eva);
DeclareResourceType(WinMenu08Eva);
DeclareResourceType(WinMenu09Eva);
DeclareResourceType(WinMenu10Eva);
DeclareResourceType(WinMenu11Eva);
DeclareResourceType(WinMenu12Eva);
DeclareResourceType(WinMenu13Eva);
DeclareResourceType(WinMenu14Eva);
DeclareResourceType(WinMenu15Eva);
DeclareResourceType(WinMenu16Eva);
DeclareResourceType(WinMenu17Eva);
DeclareResourceType(WinMenu18Eva);
DeclareResourceType(WinMenu19Eva);
DeclareResourceType(WinMenu20Eva);
DeclareResourceType(WinMenu21Eva);

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
  if (!state_is_in(GameState, GameState_Lose))
    return;

  Vec(PEvent) *keys = event_listen(Key);
  for (usize i = 0; i < keys->len; i++) {
    Key *key = (Key *)vec_index(PEvent, keys, i)->self;
    if (key->kind != Key_ESC)
      continue;
    CApp.exit();
    break;
  }
}

void global_init() {
  add_state_type(GameState);
  state_set(GameState, GameState_Setting_ReadHeight);

  add_resource_type(GameInfo);
  GameInfo *info = malloc(sizeof(GameInfo));
  info->height = 0;
  info->width = 0;
  info->food_amount = 0;
  info->mod = GameMode_Default;
  resource_insert(GameInfo, info);
  AddUpdateSystem(exit_system);

  // audio
  add_resource_type(BeginningWav);
  add_resource_type(DeathWav);
  add_resource_type(EatWav);
  add_resource_type(WalkWav);
  add_resource_type(SirenWav);
  add_resource_type(BoosterWav);
  resource_insert(BeginningWav, open_sound("assets/audio/beginning.wav"));
  resource_insert(DeathWav, open_sound("assets/audio/death.wav"));
  resource_insert(EatWav, open_sound("assets/audio/eat.wav"));
  resource_insert(WalkWav, open_sound("assets/audio/walk.wav"));
  resource_insert(SirenWav, open_sound("assets/audio/siren.wav"));
  resource_insert(BoosterWav, open_sound("assets/audio/booster.wav"));

  // pac man
  add_resource_type(PacMan1Eva);
  add_resource_type(PacMan2Eva);
  add_resource_type(PacMan3Eva);
  add_resource_type(PacMan4Eva);
  add_resource_type(PacMan5Eva);
  add_resource_type(PacMan6Eva);
  add_resource_type(PacMan7Eva);
  add_resource_type(PacMan8Eva);
  resource_insert(PacMan1Eva, open_eva("assets/pac_man/pac_man_1.eva"));
  resource_insert(PacMan2Eva, open_eva("assets/pac_man/pac_man_2.eva"));
  resource_insert(PacMan3Eva, open_eva("assets/pac_man/pac_man_3.eva"));
  resource_insert(PacMan4Eva, open_eva("assets/pac_man/pac_man_4.eva"));
  resource_insert(PacMan5Eva, open_eva("assets/pac_man/pac_man_5.eva"));
  resource_insert(PacMan6Eva, open_eva("assets/pac_man/pac_man_6.eva"));
  resource_insert(PacMan7Eva, open_eva("assets/pac_man/pac_man_7.eva"));
  resource_insert(PacMan8Eva, open_eva("assets/pac_man/pac_man_8.eva"));

  // win menu
  add_resource_type(WinMenu01Eva);
  add_resource_type(WinMenu02Eva);
  add_resource_type(WinMenu03Eva);
  add_resource_type(WinMenu04Eva);
  add_resource_type(WinMenu05Eva);
  add_resource_type(WinMenu06Eva);
  add_resource_type(WinMenu07Eva);
  add_resource_type(WinMenu08Eva);
  add_resource_type(WinMenu09Eva);
  add_resource_type(WinMenu10Eva);
  add_resource_type(WinMenu11Eva);
  add_resource_type(WinMenu12Eva);
  add_resource_type(WinMenu13Eva);
  add_resource_type(WinMenu14Eva);
  add_resource_type(WinMenu15Eva);
  add_resource_type(WinMenu16Eva);
  add_resource_type(WinMenu17Eva);
  add_resource_type(WinMenu18Eva);
  add_resource_type(WinMenu19Eva);
  add_resource_type(WinMenu20Eva);
  add_resource_type(WinMenu21Eva);
  resource_insert(WinMenu01Eva, open_eva("assets/win_menu/win_menu_01.eva"));
  resource_insert(WinMenu02Eva, open_eva("assets/win_menu/win_menu_02.eva"));
  resource_insert(WinMenu03Eva, open_eva("assets/win_menu/win_menu_03.eva"));
  resource_insert(WinMenu04Eva, open_eva("assets/win_menu/win_menu_04.eva"));
  resource_insert(WinMenu05Eva, open_eva("assets/win_menu/win_menu_05.eva"));
  resource_insert(WinMenu06Eva, open_eva("assets/win_menu/win_menu_06.eva"));
  resource_insert(WinMenu07Eva, open_eva("assets/win_menu/win_menu_07.eva"));
  resource_insert(WinMenu08Eva, open_eva("assets/win_menu/win_menu_08.eva"));
  resource_insert(WinMenu09Eva, open_eva("assets/win_menu/win_menu_09.eva"));
  resource_insert(WinMenu10Eva, open_eva("assets/win_menu/win_menu_10.eva"));
  resource_insert(WinMenu11Eva, open_eva("assets/win_menu/win_menu_11.eva"));
  resource_insert(WinMenu12Eva, open_eva("assets/win_menu/win_menu_12.eva"));
  resource_insert(WinMenu13Eva, open_eva("assets/win_menu/win_menu_13.eva"));
  resource_insert(WinMenu14Eva, open_eva("assets/win_menu/win_menu_14.eva"));
  resource_insert(WinMenu15Eva, open_eva("assets/win_menu/win_menu_15.eva"));
  resource_insert(WinMenu16Eva, open_eva("assets/win_menu/win_menu_16.eva"));
  resource_insert(WinMenu17Eva, open_eva("assets/win_menu/win_menu_17.eva"));
  resource_insert(WinMenu18Eva, open_eva("assets/win_menu/win_menu_18.eva"));
  resource_insert(WinMenu19Eva, open_eva("assets/win_menu/win_menu_19.eva"));
  resource_insert(WinMenu20Eva, open_eva("assets/win_menu/win_menu_20.eva"));
  resource_insert(WinMenu21Eva, open_eva("assets/win_menu/win_menu_21.eva"));
  
  // assets
  add_resource_type(CursorEva);
  add_resource_type(PacManEva);
  add_resource_type(BlockEva);
  add_resource_type(CherryEva);
  add_resource_type(BoosterEva);
  resource_insert(CursorEva, open_eva("assets/cursor.eva"));
  resource_insert(BlockEva, open_eva("assets/block.eva"));
  resource_insert(CherryEva, open_eva("assets/cherry.eva"));
  resource_insert(BoosterEva, open_eva("assets/booster.eva"));

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
}
