#if !defined(PLAYER_H)
#define PLAYER_H

#include <evangel/component.h>
#include <evangel/event.h>

typedef struct {
} Player;

ExportComponentType(Player);

typedef enum {
  PlayerEvent_Moved,
} PlayerEvent;

ExportEventType(PlayerEvent);

void player_init();

#endif // PLAYER_H