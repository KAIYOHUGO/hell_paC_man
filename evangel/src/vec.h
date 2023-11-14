#if !defined(__VEC_H)
#define __VEC_H

#include "basic.h"

typedef struct {
  void *ptr;
  usize len, cap, size;
} Vec;

// Vec Class
struct CVec {
  Vec (*init)(usize sizeof_T);

  void (*free)(Vec *v);

  void (*clear)(Vec *v);

  void *(*index)(Vec *v, usize index);

  void (*reserve)(Vec *v, usize additional);

  void (*resize)(Vec *v, usize new_size);

  void (*shrink_to)(Vec *v, usize new_size);

  void (*shrink_to_fit)(Vec *v);

  void *(*push)(Vec *v);

  void *(*pop)(Vec *v);

  void (*remove)(Vec *v, usize index);

  void (*swap_remove)(Vec *v, usize index);
};

extern const struct CVec CVec;

#define Vec(T) Vec

#define vec_init(T) CVec.init(sizeof(T))

// free the element first or it will cause memory leak
#define vec_free(T, v) CVec.free(v)

#define vec_clear(T, v) CVec.clear(v)

#define vec_index(T, v, i) ((T *)CVec.index(v, i))

#define vec_reserve(T, v, additional) CVec.reserve(v, additional)

#define vec_resize(T, v, new_size) CVec.resize(v, new_size)

#define vec_shrink_to(T, v) CVec.shrink_to(v)

#define vec_shrink_to_fit(T, v) CVec.shrink_to_fit(v)

#define vec_push(T, v, item) (*(T *)(CVec.push(v)) = item)

#define vec_pop(T, v) (*(T *)CVec.pop(v))

// free the element first or it will cause memory leak
#define vec_remove(T, v, i) CVec.remove(v, i)

// free the element first or it will cause memory leak
#define vec_swap_remove(T, v, i) CVec.swap_remove(v, i)

#endif // __VEC_H
