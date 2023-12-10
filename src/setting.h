#if !defined(SETTING_H)
#define SETTING_H

#include <evangel/array.h>
#include <evangel/component.h>

typedef struct {
} MapCursorDisplay;

ExportComponentType(MapCursorDisplay);

typedef struct {
} HeightDisplay;

ExportComponentType(HeightDisplay);

typedef struct {
} WidthDisplay;

ExportComponentType(WidthDisplay);

void setting_init();

#endif // SETTING_H