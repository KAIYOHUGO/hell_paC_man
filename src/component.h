#if !defined(COMPONENT_H)
#define COMPONENT_H

#include <evangel/basic.h>
#include <evangel/component.h>
#include <stdlib.h>

typedef struct {
  usize x, y;
} Position;

ExportComponentType(Position);

PComponent position_new(Position pos);

#endif // COMPONENT_H