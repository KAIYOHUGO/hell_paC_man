#if !defined(GHOST_H)
#define GHOST_H

#include "component.h"
#include <evangel/component.h>

typedef struct {
  usize cd;
} Ghost;

ExportComponentType(Ghost);

void ghost_spawn(Position pos);

void ghost_despawn();

void ghost_init();

#endif // GHOST_H