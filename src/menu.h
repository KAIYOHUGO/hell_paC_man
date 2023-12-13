#if !defined(MENU_H)
#define MENU_H

#include <evangel/component.h>

typedef struct {
} MenuDisplay;

ExportComponentType(MenuDisplay);

void menu_init();

#endif // MENU_H