#if !defined(STATE_H)
#define STATE_H

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
} GameState;

ExportResourceType(GameState);

void state_init();

#endif // STATE_H