#if !defined(__VEC_C)
#define __VEC_C

#include "vec.h"
#include "basic.h"
#include "string.h"
#include <assert.h>
#include <stdlib.h>

static Vec raw_vec_new(usize sizeof_T, usize cap) {
  void *ptr = malloc(cap * sizeof_T);
  Vec v = {
      .ptr = ptr,
      .len = 0,
      .cap = cap,
      .size = sizeof_T,
  };
  return v;
}

static void raw_vec_free(Vec *v) { free(v->ptr); }

static void *raw_vec_index(Vec *v, usize index) {
  assert(index < v->len);
  return v->ptr + index * v->size;
}

static void raw_vec_reserve(Vec *v, usize additional) {
  usize new_cap = v->cap + additional;

  void *new_ptr = malloc(new_cap * v->size);
  assert(new_ptr != NULL);

  memcpy(new_ptr, v->ptr, v->len * v->size);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_cap;
}

static void raw_vec_resize(Vec *v, usize new_size) {
  if (new_size > v->cap) {
    usize additional = max(new_size - v->cap, v->cap);
    raw_vec_reserve(v, additional);
  }
  v->len = new_size;
}

static void raw_vec_shrink_to(Vec *v, usize new_size) {
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

static void raw_vec_shrink_to_fit(Vec *v) { raw_vec_shrink_to(v, v->len); }

static void *raw_vec_push(Vec *v) {
  if (v->cap > v->len) {
    void *ret = v->ptr + v->len * v->size;
    v->len++;
    return ret;
  }
  usize additional = max(v->cap, (usize)4);
  raw_vec_reserve(v, additional);
  void *ret = v->ptr + v->len * v->size;
  v->len++;
  return ret;
}

static void *raw_vec_pop(Vec *v) {
  assert(v->len != 0);
  v->len--;
  void *ret = v->ptr + v->len;
  return ret;
}

static void raw_vec_remove(Vec *v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_vec_pop(v);
    return;
  }

  v->len--;
  memmove(v->ptr + index * v->size, v->ptr + (index + 1) * v->size,
          (v->len - index) * v->size);
  return;
}

static void raw_vec_swap_remove(Vec *v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    raw_vec_pop(v);
    return;
  }

  v->len--;
  memcpy(v->ptr + index * v->size, v->ptr + v->len * v->size, v->size);
}

struct CVec CVec = {
    .new = raw_vec_new,

    .free = raw_vec_free,

    .index = raw_vec_index,

    .reserve = raw_vec_reserve,

    .resize = raw_vec_resize,

    .shrink_to = raw_vec_shrink_to,

    .shrink_to_fit = raw_vec_shrink_to_fit,

    .push = raw_vec_push,

    .pop = raw_vec_pop,

    .remove = raw_vec_remove,

    .swap_remove = raw_vec_swap_remove,

};

#endif // __VEC_C
