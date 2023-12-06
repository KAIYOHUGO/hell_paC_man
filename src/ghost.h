#if !defined(GHOST_H)
#define GHOST_H

#include "component.h"
#include <evangel/component.h>

typedef struct {
} Ghost;

ExportComponentType(Ghost);

void ghost_spawn(Position pos);
void ghost_init();

#endif // GHOST_H