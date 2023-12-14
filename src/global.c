#include "global.h"
#include "audio.h"
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
DeclareResourceType(GhostDeadWav);
static void add_audio() {
  add_resource_type(BeginningWav);
  add_resource_type(DeathWav);
  add_resource_type(EatWav);
  add_resource_type(WalkWav);
  add_resource_type(SirenWav);
  add_resource_type(BoosterWav);
  add_resource_type(GhostDeadWav);
  resource_insert(BeginningWav, open_sound("assets/audio/beginning.wav"));
  resource_insert(DeathWav, open_sound("assets/audio/death.wav"));
  resource_insert(EatWav, open_sound("assets/audio/eat.wav"));
  resource_insert(WalkWav, open_sound("assets/audio/walk.wav"));
  resource_insert(SirenWav, open_sound("assets/audio/siren.wav"));
  resource_insert(BoosterWav, open_sound("assets/audio/booster.wav"));
  resource_insert(GhostDeadWav, open_sound("assets/audio/ghost_dead.wav"));
}

// Assets
DeclareResourceType(CursorEva);
DeclareResourceType(PacManEva);
DeclareResourceType(BlockEva);
DeclareResourceType(CherryEva);
DeclareResourceType(BoosterEva);
DeclareResourceType(SettingEva);
DeclareResourceType(GameModeDefaultEva);
DeclareResourceType(GameModeCustomEva);

// Pac man
DeclareResourceType(PacMan1Eva);
DeclareResourceType(PacMan2Eva);
DeclareResourceType(PacMan3Eva);
DeclareResourceType(PacMan4Eva);
DeclareResourceType(PacMan5Eva);
DeclareResourceType(PacMan6Eva);
DeclareResourceType(PacMan7Eva);
DeclareResourceType(PacMan8Eva);
static void add_pac_man() {
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
}

// Pac man die
DeclareResourceType(PacManDie01Eva);
DeclareResourceType(PacManDie02Eva);
DeclareResourceType(PacManDie03Eva);
DeclareResourceType(PacManDie04Eva);
DeclareResourceType(PacManDie05Eva);
DeclareResourceType(PacManDie06Eva);
DeclareResourceType(PacManDie07Eva);
DeclareResourceType(PacManDie08Eva);
DeclareResourceType(PacManDie09Eva);
DeclareResourceType(PacManDie10Eva);
DeclareResourceType(PacManDie11Eva);
static void add_pac_man_die() {
  add_resource_type(PacManDie01Eva);
  add_resource_type(PacManDie02Eva);
  add_resource_type(PacManDie03Eva);
  add_resource_type(PacManDie04Eva);
  add_resource_type(PacManDie05Eva);
  add_resource_type(PacManDie06Eva);
  add_resource_type(PacManDie07Eva);
  add_resource_type(PacManDie08Eva);
  add_resource_type(PacManDie09Eva);
  add_resource_type(PacManDie10Eva);
  add_resource_type(PacManDie11Eva);
  resource_insert(PacManDie01Eva,
                  open_eva("assets/pac_man_die/pac_man_die_01.eva"));
  resource_insert(PacManDie02Eva,
                  open_eva("assets/pac_man_die/pac_man_die_02.eva"));
  resource_insert(PacManDie03Eva,
                  open_eva("assets/pac_man_die/pac_man_die_03.eva"));
  resource_insert(PacManDie04Eva,
                  open_eva("assets/pac_man_die/pac_man_die_04.eva"));
  resource_insert(PacManDie05Eva,
                  open_eva("assets/pac_man_die/pac_man_die_05.eva"));
  resource_insert(PacManDie06Eva,
                  open_eva("assets/pac_man_die/pac_man_die_06.eva"));
  resource_insert(PacManDie07Eva,
                  open_eva("assets/pac_man_die/pac_man_die_07.eva"));
  resource_insert(PacManDie08Eva,
                  open_eva("assets/pac_man_die/pac_man_die_08.eva"));
  resource_insert(PacManDie09Eva,
                  open_eva("assets/pac_man_die/pac_man_die_09.eva"));
  resource_insert(PacManDie10Eva,
                  open_eva("assets/pac_man_die/pac_man_die_10.eva"));
  resource_insert(PacManDie11Eva,
                  open_eva("assets/pac_man_die/pac_man_die_11.eva"));
}

// Ghost
DeclareResourceType(PinkGhostEva);
DeclareResourceType(RedGhostEva);
DeclareResourceType(BlueGhostEva);
DeclareResourceType(YellowGhostEva);
static void add_ghost() {
  add_resource_type(PinkGhostEva);
  add_resource_type(RedGhostEva);
  add_resource_type(BlueGhostEva);
  add_resource_type(YellowGhostEva);
  resource_insert(PinkGhostEva, open_eva("assets/ghost/pink_ghost.eva"));
  resource_insert(RedGhostEva, open_eva("assets/ghost/red_ghost.eva"));
  resource_insert(BlueGhostEva, open_eva("assets/ghost/blue_ghost.eva"));
  resource_insert(YellowGhostEva, open_eva("assets/ghost/yellow_ghost.eva"));
}

// ghost freeze
DeclareResourceType(PinkGhostFreezeEva);
DeclareResourceType(RedGhostFreezeEva);
DeclareResourceType(BlueGhostFreezeEva);
DeclareResourceType(YellowGhostFreezeEva);
static void add_ghost_freeze() {
  add_resource_type(PinkGhostFreezeEva);
  add_resource_type(RedGhostFreezeEva);
  add_resource_type(BlueGhostFreezeEva);
  add_resource_type(YellowGhostFreezeEva);
  resource_insert(PinkGhostFreezeEva,
                  open_eva("assets/ghost/pink_ghost_freeze.eva"));
  resource_insert(RedGhostFreezeEva,
                  open_eva("assets/ghost/red_ghost_freeze.eva"));
  resource_insert(BlueGhostFreezeEva,
                  open_eva("assets/ghost/blue_ghost_freeze.eva"));
  resource_insert(YellowGhostFreezeEva,
                  open_eva("assets/ghost/yellow_ghost_freeze.eva"));
}

// ghost die
DeclareResourceType(GhostDie1Eva);
DeclareResourceType(GhostDie2Eva);
DeclareResourceType(GhostDie3Eva);
DeclareResourceType(GhostDie4Eva);
DeclareResourceType(GhostDie5Eva);
DeclareResourceType(GhostDie6Eva);
static void add_ghost_die() {
  add_resource_type(GhostDie1Eva);
  add_resource_type(GhostDie2Eva);
  add_resource_type(GhostDie3Eva);
  add_resource_type(GhostDie4Eva);
  add_resource_type(GhostDie5Eva);
  add_resource_type(GhostDie6Eva);
  resource_insert(GhostDie1Eva, open_eva("assets/ghost_die/ghost_die_1.eva"));
  resource_insert(GhostDie2Eva, open_eva("assets/ghost_die/ghost_die_2.eva"));
  resource_insert(GhostDie3Eva, open_eva("assets/ghost_die/ghost_die_3.eva"));
  resource_insert(GhostDie4Eva, open_eva("assets/ghost_die/ghost_die_4.eva"));
  resource_insert(GhostDie5Eva, open_eva("assets/ghost_die/ghost_die_5.eva"));
  resource_insert(GhostDie6Eva, open_eva("assets/ghost_die/ghost_die_6.eva"));
}

DeclareResourceType(Menu01Eva);
DeclareResourceType(Menu02Eva);
DeclareResourceType(Menu03Eva);
DeclareResourceType(Menu04Eva);
DeclareResourceType(Menu05Eva);
DeclareResourceType(Menu06Eva);
DeclareResourceType(Menu07Eva);
DeclareResourceType(Menu08Eva);
DeclareResourceType(Menu09Eva);
DeclareResourceType(Menu10Eva);
DeclareResourceType(Menu11Eva);
DeclareResourceType(Menu12Eva);
DeclareResourceType(Menu13Eva);
DeclareResourceType(Menu14Eva);
DeclareResourceType(Menu15Eva);
DeclareResourceType(Menu16Eva);
DeclareResourceType(Menu17Eva);
DeclareResourceType(Menu18Eva);
DeclareResourceType(Menu19Eva);
DeclareResourceType(Menu20Eva);
DeclareResourceType(Menu21Eva);
DeclareResourceType(Menu22Eva);
DeclareResourceType(Menu23Eva);
DeclareResourceType(Menu24Eva);
static void add_menu() {
  add_resource_type(Menu01Eva);
  add_resource_type(Menu02Eva);
  add_resource_type(Menu03Eva);
  add_resource_type(Menu04Eva);
  add_resource_type(Menu05Eva);
  add_resource_type(Menu06Eva);
  add_resource_type(Menu07Eva);
  add_resource_type(Menu08Eva);
  add_resource_type(Menu09Eva);
  add_resource_type(Menu10Eva);
  add_resource_type(Menu11Eva);
  add_resource_type(Menu12Eva);
  add_resource_type(Menu13Eva);
  add_resource_type(Menu14Eva);
  add_resource_type(Menu15Eva);
  add_resource_type(Menu16Eva);
  add_resource_type(Menu17Eva);
  add_resource_type(Menu18Eva);
  add_resource_type(Menu19Eva);
  add_resource_type(Menu20Eva);
  add_resource_type(Menu21Eva);
  add_resource_type(Menu22Eva);
  add_resource_type(Menu23Eva);
  add_resource_type(Menu24Eva);
  resource_insert(Menu01Eva, open_eva("assets/menu/menu_01.eva"));
  resource_insert(Menu02Eva, open_eva("assets/menu/menu_02.eva"));
  resource_insert(Menu03Eva, open_eva("assets/menu/menu_03.eva"));
  resource_insert(Menu04Eva, open_eva("assets/menu/menu_04.eva"));
  resource_insert(Menu05Eva, open_eva("assets/menu/menu_05.eva"));
  resource_insert(Menu06Eva, open_eva("assets/menu/menu_06.eva"));
  resource_insert(Menu07Eva, open_eva("assets/menu/menu_07.eva"));
  resource_insert(Menu08Eva, open_eva("assets/menu/menu_08.eva"));
  resource_insert(Menu09Eva, open_eva("assets/menu/menu_09.eva"));
  resource_insert(Menu10Eva, open_eva("assets/menu/menu_10.eva"));
  resource_insert(Menu11Eva, open_eva("assets/menu/menu_11.eva"));
  resource_insert(Menu12Eva, open_eva("assets/menu/menu_12.eva"));
  resource_insert(Menu13Eva, open_eva("assets/menu/menu_13.eva"));
  resource_insert(Menu14Eva, open_eva("assets/menu/menu_14.eva"));
  resource_insert(Menu15Eva, open_eva("assets/menu/menu_15.eva"));
  resource_insert(Menu16Eva, open_eva("assets/menu/menu_16.eva"));
  resource_insert(Menu17Eva, open_eva("assets/menu/menu_17.eva"));
  resource_insert(Menu18Eva, open_eva("assets/menu/menu_18.eva"));
  resource_insert(Menu19Eva, open_eva("assets/menu/menu_19.eva"));
  resource_insert(Menu20Eva, open_eva("assets/menu/menu_20.eva"));
  resource_insert(Menu21Eva, open_eva("assets/menu/menu_21.eva"));
  resource_insert(Menu22Eva, open_eva("assets/menu/menu_22.eva"));
  resource_insert(Menu23Eva, open_eva("assets/menu/menu_23.eva"));
  resource_insert(Menu24Eva, open_eva("assets/menu/menu_24.eva"));
}

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
static void add_win_menu() {
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
}

// lose menu
DeclareResourceType(LoseMenu01Eva);
DeclareResourceType(LoseMenu02Eva);
DeclareResourceType(LoseMenu03Eva);
DeclareResourceType(LoseMenu04Eva);
DeclareResourceType(LoseMenu05Eva);
DeclareResourceType(LoseMenu06Eva);
DeclareResourceType(LoseMenu07Eva);
DeclareResourceType(LoseMenu08Eva);
DeclareResourceType(LoseMenu09Eva);
DeclareResourceType(LoseMenu10Eva);
DeclareResourceType(LoseMenu11Eva);
DeclareResourceType(LoseMenu12Eva);
DeclareResourceType(LoseMenu13Eva);
DeclareResourceType(LoseMenu14Eva);
DeclareResourceType(LoseMenu15Eva);
DeclareResourceType(LoseMenu16Eva);
DeclareResourceType(LoseMenu17Eva);
static void add_lose_menu() {
  add_resource_type(LoseMenu01Eva);
  add_resource_type(LoseMenu02Eva);
  add_resource_type(LoseMenu03Eva);
  add_resource_type(LoseMenu04Eva);
  add_resource_type(LoseMenu05Eva);
  add_resource_type(LoseMenu06Eva);
  add_resource_type(LoseMenu07Eva);
  add_resource_type(LoseMenu08Eva);
  add_resource_type(LoseMenu09Eva);
  add_resource_type(LoseMenu10Eva);
  add_resource_type(LoseMenu11Eva);
  add_resource_type(LoseMenu12Eva);
  add_resource_type(LoseMenu13Eva);
  add_resource_type(LoseMenu14Eva);
  add_resource_type(LoseMenu15Eva);
  add_resource_type(LoseMenu16Eva);
  add_resource_type(LoseMenu17Eva);
  resource_insert(LoseMenu01Eva, open_eva("assets/lose_menu/lose_menu_01.eva"));
  resource_insert(LoseMenu02Eva, open_eva("assets/lose_menu/lose_menu_02.eva"));
  resource_insert(LoseMenu03Eva, open_eva("assets/lose_menu/lose_menu_03.eva"));
  resource_insert(LoseMenu04Eva, open_eva("assets/lose_menu/lose_menu_04.eva"));
  resource_insert(LoseMenu05Eva, open_eva("assets/lose_menu/lose_menu_05.eva"));
  resource_insert(LoseMenu06Eva, open_eva("assets/lose_menu/lose_menu_06.eva"));
  resource_insert(LoseMenu07Eva, open_eva("assets/lose_menu/lose_menu_07.eva"));
  resource_insert(LoseMenu08Eva, open_eva("assets/lose_menu/lose_menu_08.eva"));
  resource_insert(LoseMenu09Eva, open_eva("assets/lose_menu/lose_menu_09.eva"));
  resource_insert(LoseMenu10Eva, open_eva("assets/lose_menu/lose_menu_10.eva"));
  resource_insert(LoseMenu11Eva, open_eva("assets/lose_menu/lose_menu_11.eva"));
  resource_insert(LoseMenu12Eva, open_eva("assets/lose_menu/lose_menu_12.eva"));
  resource_insert(LoseMenu13Eva, open_eva("assets/lose_menu/lose_menu_13.eva"));
  resource_insert(LoseMenu14Eva, open_eva("assets/lose_menu/lose_menu_14.eva"));
  resource_insert(LoseMenu15Eva, open_eva("assets/lose_menu/lose_menu_15.eva"));
  resource_insert(LoseMenu16Eva, open_eva("assets/lose_menu/lose_menu_16.eva"));
  resource_insert(LoseMenu17Eva, open_eva("assets/lose_menu/lose_menu_17.eva"));
}

// Arrow
DeclareResourceType(Arrow1Eva);
DeclareResourceType(Arrow2Eva);
DeclareResourceType(Arrow3Eva);
DeclareResourceType(Arrow4Eva);
DeclareResourceType(Arrow5Eva);
static void add_arrow() {
  add_resource_type(Arrow1Eva);
  add_resource_type(Arrow2Eva);
  add_resource_type(Arrow3Eva);
  add_resource_type(Arrow4Eva);
  add_resource_type(Arrow5Eva);
  resource_insert(Arrow1Eva, open_eva("assets/arrow_1.eva"));
  resource_insert(Arrow2Eva, open_eva("assets/arrow_2.eva"));
  resource_insert(Arrow3Eva, open_eva("assets/arrow_3.eva"));
  resource_insert(Arrow4Eva, open_eva("assets/arrow_4.eva"));
  resource_insert(Arrow5Eva, open_eva("assets/arrow_5.eva"));
}

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
static void add_num() {
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

void global_init() {
  add_state_type(GameState);
  state_set(GameState, GameState_Menu);

  add_resource_type(GameInfo);
  GameInfo *info = malloc(sizeof(GameInfo));
  resource_insert(GameInfo, info);

  add_audio();

  add_pac_man();

  add_pac_man_die();

  add_menu();

  add_win_menu();

  add_lose_menu();

  add_ghost();

  add_ghost_freeze();

  add_ghost_die();

  add_arrow();

  add_num();

  // assets
  add_resource_type(CursorEva);
  add_resource_type(PacManEva);
  add_resource_type(BlockEva);
  add_resource_type(CherryEva);
  add_resource_type(BoosterEva);
  add_resource_type(SettingEva);
  add_resource_type(GameModeDefaultEva);
  add_resource_type(GameModeCustomEva);
  resource_insert(CursorEva, open_eva("assets/cursor.eva"));
  resource_insert(BlockEva, open_eva("assets/block.eva"));
  resource_insert(CherryEva, open_eva("assets/cherry.eva"));
  resource_insert(BoosterEva, open_eva("assets/booster.eva"));
  resource_insert(SettingEva, open_eva("assets/setting.eva"));
  resource_insert(GameModeDefaultEva, open_eva("assets/game_mode_default.eva"));
  resource_insert(GameModeCustomEva, open_eva("assets/game_mode_custom.eva"));
}
