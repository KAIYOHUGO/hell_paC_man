#if !defined(IN_GAME_H)
#define IN_GAME_H

#include <evangel/component.h>

#define GHost_Percentage 10
#define Food_Percentage 30
#define Booster_Percentage 10

typedef struct {
} RemainDisplay;

ExportComponentType(RemainDisplay);

typedef struct {
} HpDisplay;

ExportComponentType(LifeDisplay);

void in_game_display_despawn();

void in_game_init();

#endif // IN_GAME_H