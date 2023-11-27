#if !defined(PLAYER_C)
#define PLAYER_C

#include "player.h"
#include "component.h"
#include <stdlib.h>

DeclareComponentType(Player);

PComponent player_new(Player player) {
  Player *ptr = malloc(sizeof(Player));
  *ptr = player;
  return CComponent.default_vtable(ptr);
}

void player_init() {
  Position pos = {.x = 1, .y = 1};
  Player player = {};
  Spawn(Player, Position, player_new(player), position_new(pos));
}

#endif // PLAYER_C