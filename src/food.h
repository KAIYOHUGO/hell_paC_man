#if !defined(FOOD_H)
#define FOOD_H
#include "component.h"
#include <evangel/component.h>

typedef struct {
} Food;

ExportComponentType(Food);

void food_spawn(Position pos);

void food_despawn();

void food_init();

#endif // FOOD_H