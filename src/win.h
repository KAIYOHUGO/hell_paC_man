#if !defined(LOSE_H)
#define LOSE_H

#include <evangel/component.h>

typedef struct {
} WinDisplay;

ExportComponentType(WinDisplay);

void win_init();

#endif // LOSE_H