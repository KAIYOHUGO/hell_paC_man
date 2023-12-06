#if !defined(GLOBAL_H)
#define GLOBAL_H

#include "setting.h"
#include <evangel/resource.h>

#define SCREEN_LINE 67
#define SCREEN_HIGHT (SCREEN_LINE * 2)
#define SCREEN_WIDTH 240
#define PLAYER_SIZE 9
#define NUMBER_SIZE 9

typedef enum {
  GameState_Menu,
  GameState_Enter_Setting_ReadHeight,
  GameState_Setting_ReadHeight,
  GameState_Setting_ReadWidth,
  GameState_Exit_ReadWidth,
  GameState_Enter_Setting_ReadMap,
  GameState_Setting_ReadMap,
  GameState_Exit_Setting_ReadMap,
  GameState_InGame,

  GameState_Test,

} GameState;

ExportResourceType(GameState);

// assets
ExportResourceType(CursorEva);
ExportResourceType(PacManEva);
ExportResourceType(BlockEva);
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