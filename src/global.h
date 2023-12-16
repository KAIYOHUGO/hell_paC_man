#if !defined(GLOBAL_H)
#define GLOBAL_H

#include <evangel/resource.h>
#include <evangel/state.h>

#define SCREEN_LINE 67
#define SCREEN_HIGHT (SCREEN_LINE * 2)
#define SCREEN_WIDTH 240
#define PLAYER_SIZE 9
#define NUMBER_SIZE 9
#define PLAYER_ANIMATION_SPEED 85.0
#define SETTING_HEIGHT 45
#define SETTING_WIDTH 50
#define SETTING_ARROW_SIZE 9


typedef enum {
  GameState_Loading,
  GameState_Menu,
  // Setting
  GameState_Setting_ReadMode,
  GameState_Setting_ReadHeight,
  GameState_Setting_ReadWidth,
  GameState_Setting_ReadMap,

  // Game
  GameState_GenerateMap,
  GameState_RespawnPlayer,
  GameState_InGame,

  GameState_PlayerDie,

  // State Menu
  GameState_Win,
  GameState_Lose,
} GameState;

ExportStateType(GameState);

typedef enum {
  GameMode_Default,
  GameMode_Custom,
  _GameMode_Size,
} GameMode;

typedef struct {
  usize height, width, food_amount, life, level;
  isize offset_x, offset_y;
  GameMode mode;
} GameInfo;

ExportResourceType(GameInfo);

// audio
ExportResourceType(BeginningWav);
ExportResourceType(DeathWav);
ExportResourceType(EatWav);
ExportResourceType(WalkWav);
ExportResourceType(SirenWav);
ExportResourceType(BoosterWav);
ExportResourceType(GhostDeadWav);
ExportResourceType(SelectWav);

// assets
ExportResourceType(CursorEva);
ExportResourceType(BlockEva);
ExportResourceType(CherryEva);
ExportResourceType(BoosterEva);
ExportResourceType(SettingEva);
ExportResourceType(GameModeDefaultEva);
ExportResourceType(GameModeCustomEva);

// pac man
ExportResourceType(PacMan1Eva);
ExportResourceType(PacMan2Eva);
ExportResourceType(PacMan3Eva);
ExportResourceType(PacMan4Eva);
ExportResourceType(PacMan5Eva);
ExportResourceType(PacMan6Eva);
ExportResourceType(PacMan7Eva);
ExportResourceType(PacMan8Eva);
// pac man die
ExportResourceType(PacManDie01Eva);
ExportResourceType(PacManDie02Eva);
ExportResourceType(PacManDie03Eva);
ExportResourceType(PacManDie04Eva);
ExportResourceType(PacManDie05Eva);
ExportResourceType(PacManDie06Eva);
ExportResourceType(PacManDie07Eva);
ExportResourceType(PacManDie08Eva);
ExportResourceType(PacManDie09Eva);
ExportResourceType(PacManDie10Eva);
ExportResourceType(PacManDie11Eva);

// menu
ExportResourceType(Menu01Eva);
ExportResourceType(Menu02Eva);
ExportResourceType(Menu03Eva);
ExportResourceType(Menu04Eva);
ExportResourceType(Menu05Eva);
ExportResourceType(Menu06Eva);
ExportResourceType(Menu07Eva);
ExportResourceType(Menu08Eva);
ExportResourceType(Menu09Eva);
ExportResourceType(Menu10Eva);
ExportResourceType(Menu11Eva);
ExportResourceType(Menu12Eva);
ExportResourceType(Menu13Eva);
ExportResourceType(Menu14Eva);
ExportResourceType(Menu15Eva);
ExportResourceType(Menu16Eva);
ExportResourceType(Menu17Eva);
ExportResourceType(Menu18Eva);
ExportResourceType(Menu19Eva);
ExportResourceType(Menu20Eva);
ExportResourceType(Menu21Eva);
ExportResourceType(Menu22Eva);
ExportResourceType(Menu23Eva);
ExportResourceType(Menu24Eva);

// win menu
ExportResourceType(WinMenu01Eva);
ExportResourceType(WinMenu02Eva);
ExportResourceType(WinMenu03Eva);
ExportResourceType(WinMenu04Eva);
ExportResourceType(WinMenu05Eva);
ExportResourceType(WinMenu06Eva);
ExportResourceType(WinMenu07Eva);
ExportResourceType(WinMenu08Eva);
ExportResourceType(WinMenu09Eva);
ExportResourceType(WinMenu10Eva);
ExportResourceType(WinMenu11Eva);
ExportResourceType(WinMenu12Eva);
ExportResourceType(WinMenu13Eva);
ExportResourceType(WinMenu14Eva);
ExportResourceType(WinMenu15Eva);
ExportResourceType(WinMenu16Eva);
ExportResourceType(WinMenu17Eva);
ExportResourceType(WinMenu18Eva);
ExportResourceType(WinMenu19Eva);
ExportResourceType(WinMenu20Eva);
ExportResourceType(WinMenu21Eva);
// lose menu
ExportResourceType(LoseMenu01Eva);
ExportResourceType(LoseMenu02Eva);
ExportResourceType(LoseMenu03Eva);
ExportResourceType(LoseMenu04Eva);
ExportResourceType(LoseMenu05Eva);
ExportResourceType(LoseMenu06Eva);
ExportResourceType(LoseMenu07Eva);
ExportResourceType(LoseMenu08Eva);
ExportResourceType(LoseMenu09Eva);
ExportResourceType(LoseMenu10Eva);
ExportResourceType(LoseMenu11Eva);
ExportResourceType(LoseMenu12Eva);
ExportResourceType(LoseMenu13Eva);
ExportResourceType(LoseMenu14Eva);
ExportResourceType(LoseMenu15Eva);
ExportResourceType(LoseMenu16Eva);
ExportResourceType(LoseMenu17Eva);

// ghost
ExportResourceType(PinkGhostEva);
ExportResourceType(RedGhostEva);
ExportResourceType(BlueGhostEva);
ExportResourceType(YellowGhostEva);
// ghost freeze
ExportResourceType(PinkGhostFreezeEva);
ExportResourceType(RedGhostFreezeEva);
ExportResourceType(BlueGhostFreezeEva);
ExportResourceType(YellowGhostFreezeEva);
// ghost die
ExportResourceType(GhostDie1Eva);
ExportResourceType(GhostDie2Eva);
ExportResourceType(GhostDie3Eva);
ExportResourceType(GhostDie4Eva);
ExportResourceType(GhostDie5Eva);
ExportResourceType(GhostDie6Eva);

// arrow
ExportResourceType(Arrow1Eva);
ExportResourceType(Arrow2Eva);
ExportResourceType(Arrow3Eva);
ExportResourceType(Arrow4Eva);
ExportResourceType(Arrow5Eva);

// Num
ExportResourceType(Num0);
ExportResourceType(Num1);
ExportResourceType(Num2);
ExportResourceType(Num3);
ExportResourceType(Num4);
ExportResourceType(Num5);
ExportResourceType(Num6);
ExportResourceType(Num7);
ExportResourceType(Num8);
ExportResourceType(Num9);

void global_init();

#endif // GLOBAL_H