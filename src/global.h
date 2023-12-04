#if !defined(GLOBAL_H)
#define GLOBAL_H

#include "setting.h"
#include <evangel/resource.h>

typedef enum {
  GameState_Menu,
  // GameState_Setting,
  GameState_Setting_ReadHeight,
  GameState_Setting_ReadWidth,
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

void global_init();

#endif // GLOBAL_H