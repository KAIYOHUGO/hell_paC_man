#if !defined(COMPONENT_C)
#define COMPONENT_C

#include <evangel/component.h>

#include "component.h"

DeclareComponentType(Position);

PComponent position_new(Position pos) {
  Position *ptr = malloc(sizeof(Position));
  *ptr = pos;
  return CComponent.default_vtable(ptr);
}

#endif // COMPONENT_C