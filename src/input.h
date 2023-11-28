#if !defined(INPUT_H)
#define INPUT_H

#include <evangel/event.h>

void listen_keyboard();

typedef enum {
  Key_Unknown,

  Key_W,
  Key_A,
  Key_S,
  Key_D,
  Key_P,
  Key_B,

  // Num
  Key_0,
  Key_1,
  Key_2,
  Key_3,
  Key_4,
  Key_5,
  Key_6,
  Key_7,
  Key_8,
  Key_9,

  // special key
  Key_ENTER,
  Key_ESC,

  // Key_,
} Key;

ExportEventType(Key)

#endif // INPUT_H