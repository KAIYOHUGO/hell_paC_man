#if !defined(RENDER_C)
#define RENDER_C

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios init_terminal() {
  fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
  struct termios term, new_term;
  tcgetattr(STDIN_FILENO, &term);
  new_term = term;
  new_term.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  return term;
}

void reset_terminal(struct termios old_term) {
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  printf("\033c");
}

#endif // RENDER_C