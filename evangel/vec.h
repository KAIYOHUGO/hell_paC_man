#if !defined(__VEC_H)
#define __VEC_H

#include "basic.h"

typedef struct {
  void *ptr;
  usize len, cap, size;
} Vec;

// Vec Class
struct CVec {
  Vec (*new)(usize sizeof_T, usize cap);

  void (*free)(Vec *v);

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

extern struct CVec CVec;

#define vec_new(T, cap) CVec->new (sizeof(T), cap)

#define vec_free(T, v) CVec->free(v)

#define vec_index(T, v, index) ((T *)CVec->index(v, index))

#define vec_reserve(T, v, additional) CVec->reserve(v, additional)

#define vec_resize(T, v, new_size) CVec->resize(new_size)

#define vec_shrink_to(T, v) CVec->shrink_to(v)

#define vec_shrink_to_fit(T, v) CVec->shrink_to_fit(v)

#define vec_push(T, v, item) (*(T *)(CVec->push(v)) = item)

#define vec_pop(T, v) (*(T *)CVec->pop(v))

#define vec_remove(T, v, index) CVec->remove(v, index)

#define vec_swap_remove(T, v, index) CVec->swap_remove(v, index)

#endif // __VEC_H
