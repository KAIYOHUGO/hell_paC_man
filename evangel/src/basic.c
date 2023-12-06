#include "basic.h"
#include <stdlib.h>

usize internal_rand() { return (rand() << 16) ^ rand(); }
