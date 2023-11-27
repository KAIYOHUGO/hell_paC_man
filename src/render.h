#if !defined(RENDER_H)
#define RENDER_H

#include <termios.h>

struct termios init_terminal();

void reset_terminal(struct termios old_term);

#endif // RENDER_H