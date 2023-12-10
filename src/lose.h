#if !defined(LOSE_H)
#define LOSE_H

#include <evangel/component.h>

typedef struct {
} LoseDisplay;

ExportComponentType(LoseDisplay);

void lose_init();

#endif // LOSE_H