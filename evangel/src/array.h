#if !defined(__ARRAY_H)
#define __ARRAY_H

#include "basic.h"

typedef struct {
  void *ptr;
  usize len, size;
} Array;

struct CArray {
  Array (*init)(usize size_of_T, usize len);

  brw(Array) (*ref)(brw(void *) ptr, usize size_of_T, usize len);

  Array (*clone)(const brw(Array *) a);

  Array (*empty)();

  void (*free)(mov(Array *) a);

  void *(*index)(brw(Array *) a, usize index);
};

extern const struct CArray CArray;

#define Array(T) Array

#define array_init(T, len) CArray.init(sizeof(T), len)

#define array_clone(T, v) CArray.clone(v)

#define array_ref(ptr)                                                         \
  CArray.ref(ptr, sizeof(__typeof__(ptr[0])),                                  \
             sizeof(ptr) / sizeof(__typeof__(ptr[0])))

#define array_empty(T) CArray.empty()

#define array_free(T, a) CArray.free(a)

#define array_index(T, a, i) (T *)CArray.index(a, i)

#define array_typed(T, a) ((T *)((a)->ptr))

#endif // __ARRAY_H