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
  isize x, y, z;
} ScreenCord;

ExportComponentType(ScreenCord);

PComponent screen_cord_new(ScreenCord cord);

typedef struct {
  ResourceType eva_img;
  bool active;
} Sprite;

ExportComponentType(Sprite);

PComponent sprite_new(Sprite sprite);

typedef struct {
  usize n, len;
} Number;

ExportComponentType(Number);

PComponent number_new(Number num);

void component_init();

#endif // COMPONENT_H