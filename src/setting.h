#if !defined(SETTING_H)
#define SETTING_H

#include <evangel/array.h>
#include <evangel/component.h>

typedef struct {
} MapCursor;

ExportComponentType(MapCursor);

typedef struct {
} HeightNum;

ExportComponentType(HeightNum);

typedef struct {
} WidthNum;

ExportComponentType(WidthNum);

void setting_init();

#endif // SETTING_H