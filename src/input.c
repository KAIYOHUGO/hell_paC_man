#if !defined(INPUT_C)
#define INPUT_C

#include "input.h"
#include <evangel/basic.h>
#include <stdio.h>
#include <unistd.h>

void listen_keyboard() {
  char c[16];
  isize n = read(STDIN_FILENO, c, 10);
  if (n <= 0)
    return;
  printf("%c", c[0]);
}

#endif // INPUT_C