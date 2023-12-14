#if !defined(GHOST_H)
#define GHOST_H

#include "component.h"
#include <evangel/component.h>

typedef enum {
  GhostKind_Blue,
  GhostKind_Red,
  GhostKind_Pink,
  GhostKind_Yellow,
  _GhostKind_Size,
} GhostKind;

typedef struct {
  GhostKind kind;
  usize cd;
  bool dead;
} Ghost;

ExportComponentType(Ghost);

void ghost_spawn(Position pos);

void ghost_despawn();

void ghost_init();

#endif // GHOST_H