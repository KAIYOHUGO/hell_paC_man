#include "basic.h"


typedef usize VEC_T;
struct vec {
  VEC_T *ptr;
  usize len, cap;
};

void __vec_raw_cpy(VEC_T *src, VEC_T *des, usize len) {
  for (usize i = 0; i < len; i++) {
    des[i] = src[i];
  }
}

struct vec vec_new(usize cap) {
  VEC_T *ptr = malloc(cap * sizeof(VEC_T));
  struct vec v = {ptr, 0, cap};
  return v;
}

void vec_free(struct vec *v) { free(v->ptr); }

void vec_reserve(struct vec *v, usize additional) {
  usize new_cap = v->cap + additional;
  VEC_T *new_ptr = (VEC_T *)malloc(new_cap * sizeof(VEC_T));
  __vec_raw_cpy(v->ptr, new_ptr, v->len);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_cap;
}

void vec_resize(struct vec *v, usize new_size) {
  if (new_size > v->cap) {
    usize additional = max(new_size - v->cap, v->cap);
    vec_reserve(v, additional);
  }
  v->len = new_size;
}

void vec_shrink_to(struct vec *v, usize new_size) {
  if (new_size >= v->cap) {
    return;
  }

  VEC_T *new_ptr = (VEC_T *)malloc(new_size * sizeof(VEC_T));
  usize new_len = min(new_size, v->len);
  __vec_raw_cpy(v->ptr, new_ptr, new_len);
  free(v->ptr);
  v->ptr = new_ptr;
  v->cap = new_size;
  v->len = new_len;
}

void vec_shrink_to_fit(struct vec *v) { vec_shrink_to(v, v->len); }

void vec_push(struct vec *v, VEC_T item) {
  if (v->cap > v->len) {
    v->ptr[v->len] = item;
    v->len++;
    return;
  }
  usize additional = max(v->cap, (usize)4);
  vec_reserve(v, additional);
  v->ptr[v->len] = item;
  v->len++;
}

void vec_insert(struct vec *v, usize index, VEC_T item) {
  assert(index < v->len);
  v->len++;
  usize additional = max(v->cap, (usize)4);
  if (v->len > v->cap) {
    vec_reserve(v, additional);
  }
  __vec_raw_cpy(&v->ptr[index], &v->ptr[index + 1], v->len - index);
  v->ptr[index] = item;
}

VEC_T vec_pop(struct vec *v) {
  assert(v->len != 0);
  VEC_T ret = v->ptr[v->len];
  v->len--;
  return ret;
}

VEC_T vec_remove(struct vec *v, usize index) {
  assert(index < v->len);

  if (index == v->len - 1) {
    return vec_pop(v);
  }

  VEC_T ret = v->ptr[index];
  v->len--;
  __vec_raw_cpy(v->ptr[index + 1], v->ptr[index], v->len - index);
  return ret;
}

VEC_T vec_swap_remove(struct vec *v, usize index) {
  assert(index < v->len);
  VEC_T ret = v->ptr[index];
  v->len--;
  v->ptr[index] = v->ptr[v->len];
  return ret;
}
