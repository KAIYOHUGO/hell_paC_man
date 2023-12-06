#include "array.h"
#include <stdlib.h>
#include <string.h>

static Array raw_init(usize size_of_T, usize len) {
  void *ptr = malloc(len * size_of_T);

  Array array = {
      .ptr = ptr,
      .len = len,
      .size = size_of_T,
  };
  return array;
}

static Array raw_clone(Array *a) {
  void *ptr = malloc(a->len * a->size);
  memcpy(ptr, a->ptr, a->len * a->size);
  Array array = {
      .ptr = ptr,
      .len = a->len,
      .size = a->size,
  };
  return array;
}

static Array raw_ref(void *ptr, usize size_of_T, usize len) {
  Array array = {
      .ptr = ptr,
      .len = len,
      .size = size_of_T,
  };
  return array;
}

static Array raw_empty() {
  Array array = {
      .ptr = NULL,
      .len = 0,
      .size = 0,
  };
  return array;
}

static void raw_free(Array *a) {
  free(a->ptr);
  a->ptr = NULL;
}

static void *raw_index(Array *a, usize index) {
  return index < a->len ? a->ptr + index * a->size : NULL;
}

const struct CArray CArray = {
    .init = raw_init,

    .ref = raw_ref,

    .clone = raw_clone,

    .empty = raw_empty,

    .free = raw_free,

    .index = raw_index,
};
