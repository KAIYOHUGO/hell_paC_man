#if !defined(COMPONENT_C)
#define COMPONENT_C

#include <evangel/component.h>

#include "component.h"

DeclareComponentType(Position);

DeclareComponentType(ScreenCord);

DeclareComponentType(Sprite);

PComponent position_new(Position pos) {
  Position *ptr = malloc(sizeof(Position));
  *ptr = pos;
  return CComponent.default_vtable(ptr);
}

PComponent screen_cord_new(ScreenCord pos) {
  ScreenCord *ptr = malloc(sizeof(ScreenCord));
  *ptr = pos;
  return CComponent.default_vtable(ptr);
}

void component_init() {
  add_component_type(Position);
  add_component_type(ScreenCord);
  add_component_type(Sprite);
}

#endif // COMPONENT_C