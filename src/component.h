#if !defined(COMPONENT_H)
#define COMPONENT_H

#include <evangel/basic.h>
#include <evangel/component.h>
#include <evangel/resource.h>
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

typedef struct {
  ResourceType eva_img;
} Sprite;

ExportComponentType(Sprite);

void component_init();

#endif // COMPONENT_H