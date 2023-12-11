#if !defined(WIN_H)
#define WIN_H

#define WIN_HEIGHT 35 
#define WIN_WIDTH 82 

#include <evangel/component.h>

typedef struct {
} WinDisplay;

ExportComponentType(WinDisplay);

void win_init();

#endif // WIN_H