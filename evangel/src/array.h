#if !defined(__ARRAY_H)
#define __ARRAY_H

#include "basic.h"

typedef struct {
  void *ptr;
  usize len, size;
} Array;

struct CArray {
  Array (*init)(usize size_of_T, usize len);

  Array (*ref)(borrow_ptr(void *) ptr, usize size_of_T, usize len);

  void (*free)(Array *a);

  void *(*index)(Array *a, usize index);
};

extern const struct CArray CArray;

#define Array(T) Array

#define array_init(T, len) CArray.init(sizeof(T), len)

#define array_ref(T, ptr, len) CArray.ref(ptr, sizeof(T), len)

#define array_free(T, a) CArray.free(a)

#define array_index(T, a, i) (T *)CArray.index(a, i)

#define array_typed(T, a) ((T *)((a)->ptr))

#endif // __ARRAY_H