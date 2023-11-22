#if !defined(__ARRAY_C)
#define __ARRAY_C

#include "array.h"
#include <stdlib.h>

static Array raw_init(usize size_of_T, usize len) {
  void *ptr = malloc(len * size_of_T);

  Array array = {
      .ptr = ptr,
      .len = len,
      .size = size_of_T,
  };
  return array;
}

static void raw_free(Array *a) { free(a->ptr); }

static void *raw_index(Array *a, usize index) {
  return index < a->len ? a->ptr + index * a->size : NULL;
}

const struct CArray CArray = {
    .init = raw_init,
    .free = raw_free,
    .index = raw_index,
};

#endif // __ARRAY_C