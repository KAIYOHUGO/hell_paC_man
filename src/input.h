#if !defined(INPUT_H)
#define INPUT_H

#include <evangel/event.h>

void listen_keyboard();

typedef enum {
  Key_Unknown,

  // a,b,c...
  Key_Char,

  Key_Number,

  // special key
  Key_ENTER,
  Key_ESC,

  // Key_,
} KeyKind;

typedef struct {
  KeyKind kind;
  union {
    char charater;
    u8 number;
  };
} Key;

ExportEventType(Key)

#endif // INPUT_H