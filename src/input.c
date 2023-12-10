#include "input.h"
#include <evangel/event.h>
#include <stdlib.h>
#include <unistd.h>

DeclareEventType(Key);

// https://web.archive.org/web/20170407122137/http://cc.byexamples.com/2007/04/08/non-blocking-user-input-in-loop-without-ncurses/
bool input_ready() {
  struct timeval tv = {};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds));
}

void listen_keyboard() {
  if (!input_ready())
    return;

  char c[64];
  isize n = read(STDIN_FILENO, c, 64);
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