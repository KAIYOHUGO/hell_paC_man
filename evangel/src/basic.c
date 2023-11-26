#if !defined(__BASIC_C)
#define __BASIC_C

#include "basic.h"
#include <stdlib.h>

usize internal_rand() { return (rand() << 16) ^ rand(); }

#endif // __BASIC_C