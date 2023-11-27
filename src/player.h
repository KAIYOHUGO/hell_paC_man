#if !defined(PLAYER_H)
#define PLAYER_H

#include <evangel/component.h>

typedef struct {
} Player;

ExportComponentType(Player);

void player_init();

#endif // PLAYER_H