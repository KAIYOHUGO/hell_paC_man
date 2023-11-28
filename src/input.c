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
    Key key = Key_Unknown;
    switch (c[i]) {
    case 'w':
    case 'W':
      key = Key_W;
      break;
    case 'a':
    case 'A':
      key = Key_A;
      break;
    case 's':
    case 'S':
      key = Key_S;
      break;
    case 'd':
    case 'D':
      key = Key_D;
      break;

    // Num
    case '0':
      key = Key_0;
      break;
    case '1':
      key = Key_1;
      break;
    case '2':
      key = Key_2;
      break;
    case '3':
      key = Key_3;
      break;
    case '4':
      key = Key_4;
      break;
    case '5':
      key = Key_5;
      break;
    case '6':
      key = Key_6;
      break;
    case '7':
      key = Key_7;
      break;
    case '8':
      key = Key_8;
      break;
    case '9':
      key = Key_9;
      break;

    // special key
    case '\n':
      key = Key_ENTER;
      break;
    case 27:
      key = Key_ESC;
      break;
    }

    if (key != Key_Unknown) {
      Key *ptr = malloc(sizeof(Key));
      *ptr = key;
      event_emit(Key, CEvent.default_vtable(ptr));
    }
  }
}

#endif // INPUT_C