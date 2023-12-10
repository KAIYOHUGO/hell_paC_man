#if !defined(PLAYER_H)
#define PLAYER_H

#include "component.h"
#include <evangel/component.h>
#include <evangel/event.h>

typedef struct {
} Player;

ExportComponentType(Player);

typedef enum {
  PlayerEvent_Moved,
  PlayerEvent_Eat,
  PlayerEvent_Die,
} PlayerEvent;

ExportEventType(PlayerEvent);

void player_spawn(Position pos);

void player_move(Position pos);

void player_despawn();

void player_init();

#endif // PLAYER_H