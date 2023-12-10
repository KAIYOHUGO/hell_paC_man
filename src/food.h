#if !defined(FOOD_H)
#define FOOD_H
#include <evangel/component.h>
#include "component.h"

typedef struct {
} Food;

ExportComponentType(Food);

void food_spawn(Position pos);

void food_init();

#endif // FOOD_H