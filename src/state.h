#if !defined(STATE_H)
#define STATE_H

#include <evangel/resource.h>

typedef enum {
  GameState_Menu,
  GameState_Setting,
  GameState_InGame,
} GameState;

ExportResourceType(GameState)

#endif // STATE_H