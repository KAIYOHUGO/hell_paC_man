#if !defined(SETTING_H)
#define SETTING_H

#include <evangel/array.h>
#include <evangel/resource.h>

typedef enum {
  MapItem_Empty,
  MapItem_Player,
  MapItem_Ghost,
  MapItem_Booster,
} MapItem;

typedef struct {
  usize height, width;
  Array(MapItem) map;
} GameInfo;

ExportResourceType(GameInfo)

#endif // SETTING_H