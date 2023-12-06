#include "vec.h"
#include "basic.h"
#include "string.h"
#include <assert.h>
#include <stdlib.h>

static Vec raw_init(usize sizeof_T) {
  Vec v = {
      .ptr = NULL,
      .len = 0,
      .cap = 0,
      .size = sizeof_T,
  };
  return v;
}

static void raw_free(Vec *v) { free(v->ptr); }

static Vec raw_clone(Vec *v) {
  if (v->len == 0) {
    return raw_init(v->size);
  }
  usize size = v->len * v->size;
  void *ptr = malloc(size);
  memcpy(ptr, v->ptr, size);
  Vec ret = {
      .ptr = ptr,
      .len = v->len,
      .cap = v->len,
      .size = v->size,
  };
  return ret;
}

static void raw_clear(Vec *v) { v->len = 0; }

static void *raw_index(Vec *v, usize index) {
  return index < v->len ? v->ptr + index * v->size : NULL;
}

static void raw_reserve(Vec *v, usize additional) {
  usize new_cap = v->cap + additional;

  void *new_ptr = malloc(new_cap * v->size);
  assert(new_ptr != NULL);

  memcpy(new_ptr, v->ptr, v->len * v->size);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_cap;
}

static void raw_resize(Vec *v, usize new_size) {
  if (new_size > v->cap) {
    usize additional = max(new_size - v->cap, max(v->cap, 4));
    raw_reserve(v, additional);
  }
  v->len = new_size;
}

static void raw_shrink_to(Vec *v, usize new_size) {
  if (new_size >= v->cap) {
    return;
  }

  void *new_ptr = malloc(new_size * v->size);
  usize new_len = min(new_size, v->len);
  memcpy(new_ptr, v->ptr, new_len);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_size;
  v->len = new_len;
}

static void raw_shrink_to_fit(Vec *v) { raw_shrink_to(v, v->len); }

static void *raw_push(Vec *v) {
  if (v->cap > v->len) {
    void *ret = v->ptr + v->len * v->size;
    v->len++;
    return ret;
  }
  usize additional = max(v->cap, (usize)4);
  raw_reserve(v, additional);
  void *ret = v->ptr + v->len * v->size;
  v->len++;
  return ret;
}

static void *raw_pop(Vec *v) {
  assert(v->len != 0);

  v->len--;
  void *ret = v->ptr + v->len * v->size;
  return ret;
}

static void raw_remove(Vec *v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_pop(v);
    return;
  }

  v->len--;
  memmove(v->ptr + index * v->size, v->ptr + (index + 1) * v->size,
          (v->len - index) * v->size);
  return;
}

static void raw_swap_remove(Vec *v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_pop(v);
    return;
  }

  v->len--;
  memcpy(v->ptr + index * v->size, v->ptr + v->len * v->size, v->size);
}

const struct CVec CVec = {
    .init = raw_init,

    .free = raw_free,

    .clone = raw_clone,

    .clear = raw_clear,

    .index = raw_index,

    .reserve = raw_reserve,

    .resize = raw_resize,

    .shrink_to = raw_shrink_to,

    .shrink_to_fit = raw_shrink_to_fit,

    .push = raw_push,

    .pop = raw_pop,

    .remove = raw_remove,

    .swap_remove = raw_swap_remove,

};
