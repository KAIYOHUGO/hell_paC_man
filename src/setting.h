#if !defined(SETTING_H)
#define SETTING_H

#include <evangel/array.h>
#include <evangel/component.h>
#include <evangel/resource.h>

typedef enum {
  MapItem_Empty,
  MapItem_Player,
  MapItem_Monster,
  MapItem_Booster,
  MapItem_Food,
} MapItem;

typedef struct {
  usize height, width;
  isize offset_x, offset_y;
} GameInfo;

ExportResourceType(GameInfo);

typedef struct {
} MapCursor;

ExportComponentType(MapCursor);

void setting_init();

#endif // SETTING_H