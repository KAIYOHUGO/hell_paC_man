#if !defined(COMPONENT_H)
#define COMPONENT_H

#include <evangel/basic.h>
#include <evangel/component.h>
#include <stdlib.h>

typedef struct {
  isize x, y;
} Position;

ExportComponentType(Position);

PComponent position_new(Position pos);

typedef struct {
  isize x, y;
} ScreenCord;

ExportComponentType(ScreenCord);

PComponent screen_cord_new(ScreenCord pos);

#endif // COMPONENT_H