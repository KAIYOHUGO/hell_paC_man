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

typedef enum {
  GameState_Menu,
  // Setting
  GameState_Setting_ReadHeight,
  GameState_Setting_ReadWidth,
  GameState_Setting_ReadMap,

  // Game
  GameState_GenerateMap,
  GameState_RespawnPlayer,
  GameState_InGame,

  // End
  GameState_Win,
  GameState_Lose,

  GameState_Test,

} GameState;

ExportStateType(GameState);

typedef enum {
  GameMode_Default,
  GameMode_Custom,
} GameMode;

typedef struct {
  usize height, width, food_amount, life;
  isize offset_x, offset_y;
  GameMode mod;
} GameInfo;

ExportResourceType(GameInfo);

// audio
ExportResourceType(BeginningWav);
ExportResourceType(DeathWav);
ExportResourceType(EatWav);
ExportResourceType(WalkWav);
ExportResourceType(SirenWav);
ExportResourceType(BoosterWav);

// assets
ExportResourceType(CursorEva);
ExportResourceType(BlockEva);
ExportResourceType(CherryEva);
ExportResourceType(BoosterEva);
// pac man
ExportResourceType(PacMan1Eva);
ExportResourceType(PacMan2Eva);
ExportResourceType(PacMan3Eva);
ExportResourceType(PacMan4Eva);
ExportResourceType(PacMan5Eva);
ExportResourceType(PacMan6Eva);
ExportResourceType(PacMan7Eva);
ExportResourceType(PacMan8Eva);
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

// ghost
ExportResourceType(PinkGhostEva);

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