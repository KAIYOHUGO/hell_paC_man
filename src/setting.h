#if !defined(SETTING_H)
#define SETTING_H

#include <evangel/array.h>
#include <evangel/component.h>

typedef struct {
} ArrowDisplay;

ExportComponentType(ArrowDisplay);

typedef struct {
} MapCursorDisplay;

ExportComponentType(MapCursorDisplay);

typedef struct {
} ModeDisplay;

ExportComponentType(ModeDisplay);

typedef struct {
} HeightDisplay;

ExportComponentType(HeightDisplay);

typedef struct {
} WidthDisplay;

ExportComponentType(WidthDisplay);

typedef struct {
} BackgroundText;

ExportComponentType(BackgroundText);

typedef struct {
} BackgroundBlock;

ExportComponentType(BackgroundBlock);

void setting_init();

#endif // SETTING_H