#include <stdlib.h>
#if !defined(INPUT_C)
#define INPUT_C

#include "input.h"
#include <evangel/event.h>
#include <unistd.h>

DeclareEventType(Key);

void listen_keyboard() {
  char c[16];
  isize n = read(STDIN_FILENO, c, 10);
  if (n <= 0)
    return;
  for (usize i = 0; i < n; i++) {
    char keycode = c[i];
    Key key;
    key.kind = Key_Unknown;
    switch (keycode) {
    // special key
    case '\n':
      key.kind = Key_ENTER;
      break;
    case 27:
      key.kind = Key_ESC;
      break;
    default:
      if ('0' <= keycode && keycode <= '9') {
        key.kind = Key_Number;
        key.number = keycode - '0';
      } else {
        key.kind = Key_Char;
        key.charater = keycode;
      }
    }

    if (key.kind != Key_Unknown) {
      Key *ptr = malloc(sizeof(Key));
      *ptr = key;
      event_emit(Key, CEvent.default_vtable(ptr));
    }
  }
}

#endif // INPUT_C