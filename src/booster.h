#if !defined(BOOSTER_H)
#define BOOSTER_H

#include "component.h"
#include <evangel/app.h>
#include <evangel/component.h>
#include <evangel/event.h>

typedef struct {
} Booster;

ExportComponentType(Booster);

void booster_spawn(Position pos);

typedef enum {
  BoosterKind_Freeze,
  BoosterKind_LessGhost,
  BoosterKind_ExtraLife,
  // BoosterKind_Power,
  _BoosterKind_End
} BoosterEvent;

ExportEventType(BoosterEvent);

void booster_init();

#endif // BOOSTER_H