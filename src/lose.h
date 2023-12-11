#if !defined(LOSE_H)
#define LOSE_H

#define LOSE_HEIGHT 35
#define LOSE_WIDTH 82

#include <evangel/component.h>

typedef struct {
} LoseDisplay;

ExportComponentType(LoseDisplay);

void lose_init();

#endif // LOSE_H